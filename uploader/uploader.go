package main

import "time"
import "fmt"
import "strings"
import "strconv"
import "bufio"
import "os"

func transmitValue(time time.Time, temperature float64, ph float64) {
	fmt.Println("Sending Value:", time, temperature, ph)
}

const format = "2006-01-02+15:04:05"

// TODO: Error Handling

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	for scanner.Scan() {
		input := scanner.Text()
		parts := strings.Split(input, " ")

		time, _ := time.Parse(format, parts[0])
		temperature, _ := strconv.ParseFloat(parts[1], 64)
		ph, _ := strconv.ParseFloat(parts[2], 64)

		go transmitValue(time, temperature, ph)
	}
}
