#include "TROOT.h"
#include "TCutG.h"
#include "TFile.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <set>

void read_event_csv()
{
  string run_eventID_string;
  long run_eventID;
  ifstream fragCut_adc_events_csvfile ("/p/lscratche/jerbundg/analysis/pid/eventID_csv/fragCut_adc_events.csv");
  set<long> fragCut_adc_run_event_set;
  while (fragCut_adc_events_csvfile.good() ) {
    getline(fragCut_adc_events_csvfile,run_eventID_string);
    run_eventID = atol(run_eventID_string.c_str());
    fragCut_adc_run_event_set.insert(run_eventID);
  }
  ifstream fragCut_unlikely_adc_events_csvfile ("/p/lscratche/jerbundg/analysis/pid/eventID_csv/fragCut_unlikely_adc_events.csv");
  set<long> fragCut_unlikely_adc_run_event_set;
  while (fragCut_unlikely_adc_events_csvfile.good() ) {
    getline(fragCut_unlikely_adc_events_csvfile,run_eventID_string);
    run_eventID = atol(run_eventID_string.c_str());
    fragCut_unlikely_adc_run_event_set.insert(run_eventID);
  }
  ifstream alphaCut_zCut_adc_events_csvfile ("/p/lscratche/jerbundg/analysis/pid/eventID_csv/alphaCut_zCut_adc_events.csv");
  set<long> alphaCut_zCut_adc_run_event_set;
  while (alphaCut_zCut_adc_events_csvfile.good() ) {
    getline(alphaCut_zCut_adc_events_csvfile,run_eventID_string);
    run_eventID = atol(run_eventID_string.c_str());
    alphaCut_zCut_adc_run_event_set.insert(run_eventID);
  }
  ifstream alphaCut_long_low_adc_events_csvfile ("/p/lscratche/jerbundg/analysis/pid/eventID_csv/alphaCut_long_low_adc_events.csv");
  set<long> alphaCut_long_low_adc_run_event_set;
  while (alphaCut_long_low_adc_events_csvfile.good() ) {
    getline(alphaCut_long_low_adc_events_csvfile,run_eventID_string);
    run_eventID = atol(run_eventID_string.c_str());
    alphaCut_long_low_adc_run_event_set.insert(run_eventID);
  }
  ifstream alphaCut_long_adc_events_csvfile ("/p/lscratche/jerbundg/analysis/pid/eventID_csv/alphaCut_long_adc_events.csv");
  set<long> alphaCut_long_adc_run_event_set;
  while (alphaCut_long_adc_events_csvfile.good() ) {
    getline(alphaCut_long_adc_events_csvfile,run_eventID_string);
    run_eventID = atol(run_eventID_string.c_str());
    alphaCut_long_adc_run_event_set.insert(run_eventID);
  }
  ifstream long_high_adc_events_csvfile ("/p/lscratche/jerbundg/analysis/pid/eventID_csv/long_high_adc_events.csv");
  set<long> long_high_adc_run_event_set;
  while (long_high_adc_events_csvfile.good() ) {
    getline(long_high_adc_events_csvfile,run_eventID_string);
    run_eventID = atol(run_eventID_string.c_str());
    long_high_adc_run_event_set.insert(run_eventID);
  }
  ifstream alphaCut_short_higher_adc_events_csvfile ("/p/lscratche/jerbundg/analysis/pid/eventID_csv/alphaCut_short_higher_adc_events.csv");
  set<long> alphaCut_short_higher_adc_run_event_set;
  while (alphaCut_short_higher_adc_events_csvfile.good() ) {
    getline(alphaCut_short_higher_adc_events_csvfile,run_eventID_string);
    run_eventID = atol(run_eventID_string.c_str());
    alphaCut_short_higher_adc_run_event_set.insert(run_eventID);
  }
  ifstream alphaCut_short_low_adc_events_csvfile ("/p/lscratche/jerbundg/analysis/pid/eventID_csv/alphaCut_short_low_adc_events.csv");
  set<long> alphaCut_short_low_adc_run_event_set;
  while (alphaCut_short_low_adc_events_csvfile.good() ) {
    getline(alphaCut_short_low_adc_events_csvfile,run_eventID_string);
    run_eventID = atol(run_eventID_string.c_str());
    alphaCut_short_low_adc_run_event_set.insert(run_eventID);
  }
  ifstream blobs_events_csvfile ("/p/lscratche/jerbundg/analysis/pid/eventID_csv/blobs_events.csv");
  set<long> blobs_run_event_set;
  while (blobs_events_csvfile.good() ) {
    getline(blobs_events_csvfile,run_eventID_string);
    run_eventID = atol(run_eventID_string.c_str());
    blobs_run_event_set.insert(run_eventID);
  }

  fragCut_adc_events_csvfile.close();
  fragCut_unlikely_adc_events_csvfile.close();
  alphaCut_zCut_adc_events_csvfile.close();
  alphaCut_long_low_adc_events_csvfile.close();
  alphaCut_long_adc_events_csvfile.close();
  long_high_adc_events_csvfile.close();
  alphaCut_short_higher_adc_events_csvfile.close();
  alphaCut_short_low_adc_events_csvfile.close();
  blobs_events_csvfile.close();

}
