#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  long hours = seconds / 3600;
  long minutes = (seconds % 3600) / 60;
  long new_seconds = (seconds % 3600) % 60;
  string time;
  if(hours <= 9) { time = "0" + hours; }
  else { time = hours; }
  if(minutes <= 9) { time += ":0" + minutes; }
  else { time += ":" + hours; }
  if(new_seconds <= 9) { time += ":0" + new_seconds; }
  else { time += ":" + new_seconds; }
  return string(); }