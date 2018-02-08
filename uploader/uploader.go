package main

import (
	"bufio"
	"errors"
	"fmt"
	"net/http"
	"os"
	"strconv"
	"strings"
	"time"

	"github.com/beeker1121/goque"
	"k8s.io/apimachinery/pkg/util/wait"
)

const format = "2006-01-02+15:04:05"

type myError struct {
	input string
	e     error
}

func transmitValue(time time.Time, temperature float64, ph float64) (err error) {
	fmt.Println("Sending Value:", time, temperature, ph)

	timeStr := time.Format(format)
	tempStr := strconv.FormatFloat(temperature, 'f', -1, 64)
	phStr := strconv.FormatFloat(ph, 'f', -1, 64)

	url := "https://idp.s-paarmann.de/api/insert.php?table=measurements&time=" + timeStr + "&temperature=" + tempStr + "&ph=" + phStr + "&key=sYc2UhPSlCMF7OwtY4Xd"

	_, e := http.Get(url)
	if e != nil {
		err = e
	} else {
		err = nil
	}

	return
}

func handleParseErrors(errors chan myError) {
	errorlog, e := os.OpenFile("uploader_errors.txt", os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
	if e != nil {
		panic("Could not open errorlog file!")
	}
	defer errorlog.Close()

	for {
		pError := <-errors

		line := fmt.Sprintln("Error on '"+pError.input+"':", pError.e)

		fmt.Print(line)
		errorlog.WriteString(line)
	}
}

func handleInputs(inputs *goque.Queue, errorChan chan myError) {
	for {
		wait.PollInfinite(time.Duration(1)*time.Second, func() (done bool, err error) {
			done = inputs.Length() > 0
			err = nil
			return
		})

		item, e := inputs.Dequeue()
		if e != nil {
			fmt.Println("Error dequeing item!", e)
			continue
		}
		input := item.ToString()
		parts := strings.Split(input, " ")

		if len(parts) != 3 {
			errorChan <- myError{input, errors.New("Couldn't split into 3 parts")}
			continue
		}

		time, e := time.Parse(format, parts[0])
		if e != nil {
			errorChan <- myError{input, e}
			continue
		}

		temperature, e := strconv.ParseFloat(parts[1], 64)
		if e != nil {
			errorChan <- myError{input, e}
			continue
		}

		ph, e := strconv.ParseFloat(parts[2], 64)
		if e != nil {
			errorChan <- myError{input, e}
			continue
		}

		e = transmitValue(time, temperature, ph)
		if e != nil {
			errorChan <- myError{input, e}
			inputs.EnqueueString(input)
		}
	}
}

// Main is basically just responsible for passing
// input along to handleInputs and some glue connecting the various channels.
// it's pretty much an abstraction over the actual method of getting
// input so it can easily be changed.
func main() {
	//inputs := make(chan string, 5)
	errors := make(chan myError, 5)

	q, e := goque.OpenQueue("uploader_data")
	if e != nil {
		panic("Could not open queue!")
	}
	defer q.Close()

	go handleParseErrors(errors)
	go handleInputs(q, errors)

	scanner := bufio.NewScanner(os.Stdin)
	for scanner.Scan() {
		input := scanner.Text()

		_, e := q.EnqueueString(input)
		if e != nil {
			fmt.Println("Error enqueing input!", e)
		}
	}
}
