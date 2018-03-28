#include <cstdio>
#include <string>

class Output {
  private:
    FILE *file;
  public:
    Output(std::string fileName);
    ~Output();
    void Log(float temp, float ph);
};
