#include "processor.h"
#include "linux_parser.h"
#include <dirent.h>
#include <unistd.h>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { return LinuxParser::CpuUtilization(); }
