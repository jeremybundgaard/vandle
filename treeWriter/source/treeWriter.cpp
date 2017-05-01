
#include <iostream>

#include <unistd.h>
#include <getopt.h>
#include <cstring>

#include "XiaData.hpp"

// Local files
#include "treeWriter.hpp"

// Root
#include "TApplication.h"
#include "TAxis.h"
#include "TTree.h"
#include "TFile.h"

// Define the name of the program.
#ifndef PROG_NAME
#define PROG_NAME "treeWriter"
#endif

unsigned int rawEventsRead = 0;

void displayBool(const char *msg_, const bool &val_){
	if(val_) std::cout << msg_ << "YES\n";
	else std::cout << msg_ << "NO\n";
}

///////////////////////////////////////////////////////////////////////////////
// class readerUnpacker
///////////////////////////////////////////////////////////////////////////////

/** Process all events in the event list.
  * \param[in]  addr_ Pointer to a location in memory.
  * \return Nothing.
  */
void readerUnpacker::ProcessRawEvent(ScanInterface *addr_/*=NULL*/){
	if(!addr_){ return; }

	XiaData *current_event = NULL;

	// Fill the processor event deques with events
	while(!rawEvent.empty()){
		if(!running) break;

		current_event = rawEvent.front();
		rawEvent.pop_front(); // Remove this event from the raw event deque.

		// Check that this channel event exists.
		if(!current_event){ continue; }

		// Send the event to the scan interface object for processing.
		if(addr_->AddEvent(current_event))
			addr_->ProcessEvents();
	}

	// Finish up with this raw event.
	addr_->ProcessEvents();

	// Increment the number of raw events processed so far.
	rawEventsRead++;
}

///////////////////////////////////////////////////////////////////////////////
// class readerScanner
///////////////////////////////////////////////////////////////////////////////

/// Default constructor.
readerScanner::readerScanner() : ScanInterface(), init(false), showFlags(false), showTrace(false), showNextEvent(false), numSkip(0), eventsRead(0) {
	currentEvent = new XiaData();
}

/// Destructor.
readerScanner::~readerScanner(){
	if(init){ // Handle some cleanup.
	}
	file_channelEventTree->Close();
	delete currentEvent;
}

/** ExtraCommands is used to send command strings to classes derived
  * from ScanInterface. If ScanInterface receives an unrecognized
  * command from the user, it will pass it on to the derived class.
  * \param[in]  cmd_ The command to interpret.
  * \param[out] arg_ Vector or arguments to the user command.
  * \return True if the command was recognized and false otherwise.
  */
bool readerScanner::ExtraCommands(const std::string &cmd_, std::vector<std::string> &args_){
	if(cmd_ == "flags"){
		showFlags = !showFlags;
	}
	else if(cmd_ == "trace"){
		showTrace = !showTrace;
	}
	else if(cmd_ == "next" || cmd_ == "n"){
		if(args_.size() >= 1){ // Skip the specified number of events.
			showNextEvent = true;
			numSkip = strtoul(args_.at(0).c_str(), NULL, 0);
			std::cout << msgHeader << "Skipping " << numSkip << " events.\n";
		}
		else{ showNextEvent = true; }
	}
	else{ return false; } // Unrecognized command.

	return true;
}

/** ExtraArguments is used to send command line arguments to classes derived
  * from ScanInterface. This method should loop over the optionExt elements
  * in the vector userOpts and check for those options which have been flagged
  * as active by ::Setup(). This should be overloaded in the derived class.
  * \return Nothing.
  */
void readerScanner::ExtraArguments(){
	if(userOpts.at(0).active){
		numSkip = strtoul(userOpts.at(0).argument.c_str(), NULL, 0);
		std::cout << msgHeader << "Skipping " << numSkip << " events.\n";
	}
	/*if(userOpts.at(1).active)
		std::cout << msgHeader << "Using option --myarg2 (-y): arg=\"" << userOpts.at(1).argument << "\"\n";
	if(userOpts.at(2).active)
		std::cout << msgHeader << "Using option --myarg3 (-z): arg=\"" << userOpts.at(2).argument << "\"\n";
	if(userOpts.at(3).active)
		std::cout << msgHeader << "Using option --myarg4.\n";*/
}

/** CmdHelp is used to allow a derived class to print a help statement about
  * its own commands. This method is called whenever the user enters 'help'
  * or 'h' into the interactive terminal (if available).
  * \param[in]  prefix_ String to append at the start of any output. Not used by default.
  * \return Nothing.
  */
void readerScanner::CmdHelp(const std::string &prefix_/*=""*/){
	std::cout << "   flags               - Display channel flags.\n";
	std::cout << "   trace               - Print adc trace values.\n";
	std::cout << "   draw                - Draw the adc trace to the screen.\n";
	std::cout << "   next (n) [N=0]      - Skip N events and display the next one.\n";
}

/** ArgHelp is used to allow a derived class to add a command line option
  * to the main list of options. This method is called at the end of
  * from the ::Setup method.
  * Does nothing useful by default.
  * \return Nothing.
  */
void readerScanner::ArgHelp(){
	AddOption(optionExt("skip", required_argument, NULL, 'S', "<N>", "Skip the first N events in the input file."));
	/*AddOption(optionExt("myarg2", required_argument, NULL, 'y', "<arg>", "A useful command line argument with a required argument."));
	AddOption(optionExt("myarg3", optional_argument, NULL, 'z', "[arg]", "A useful command line argument with an optional argument."));
	AddOption(optionExt("myarg4", no_argument, NULL, 0, "", "A long only command line argument."));*/

	// Note that the following single character options are reserved by ScanInterface
	//  b, h, i, o, q, s, and v
}

/** SyntaxStr is used to print a linux style usage message to the screen.
  * \param[in]  name_ The name of the program.
  * \return Nothing.
  */
void readerScanner::SyntaxStr(char *name_){
	std::cout << " usage: " << std::string(name_) << " [options]\n";
}

/** Initialize the map file, the config file, the processor handler,
  * and add all of the required processors.
  * \param[in]  prefix_ String to append to the beginning of system output.
  * \return True upon successfully initializing and false otherwise.
  */
bool readerScanner::Initialize(std::string prefix_){
	if(init){ return false; }

	// Do some initialization.
	file_channelEventTree = new TFile("channelEventTree.root","recreate");
	channelEventTree = new TTree("channelEventTree","channelEventTree");

	channelEventTree->Branch("Trigger_Time",&Trigger_Time);
	channelEventTree->Branch("Filter_Energy",&Filter_Energy);
	channelEventTree->Branch("Event_Time_Lo",&Event_Time_Lo);
	channelEventTree->Branch("Event_Time_Hi",&Event_Time_Hi);
	channelEventTree->Branch("Crate",&Crate);
	channelEventTree->Branch("Slot",&Slot);
	channelEventTree->Branch("Module",&Module);
	channelEventTree->Branch("Channel",&Channel);
	channelEventTree->Branch("CFD_Time",&CFD_Time);
	channelEventTree->Branch("size_t Trace_Length",&Trace_Length);
	channelEventTree->Branch("adc_Trace",&adc_Trace);
	channelEventTree->Branch("num_Qdcs",&num_Qdcs);
	channelEventTree->Branch("qdc_Value",&qdc_Value);
	channelEventTree->Branch("Virtual",&Virtual);
	channelEventTree->Branch("Pileup",&Pileup);
	channelEventTree->Branch("Saturated",&Saturated);
	channelEventTree->Branch("CFD_Force",&CFD_Force);
	channelEventTree->Branch("CFD_Trig",&CFD_Trig);

	return (init = true);
}

/** Peform any last minute initialization before processing data.
  * /return Nothing.
  */
void readerScanner::FinalInitialization(){
	// Do some last minute initialization before the run starts.
}

/** Receive various status notifications from the scan.
  * \param[in] code_ The notification code passed from ScanInterface methods.
  * \return Nothing.
  */
void readerScanner::Notify(const std::string &code_/*=""*/){
	if(code_ == "START_SCAN"){  }
	else if(code_ == "STOP_SCAN"){  }
	else if(code_ == "SCAN_COMPLETE"){ std::cout << msgHeader << "Scan complete.\n"; }
	else if(code_ == "LOAD_FILE"){ std::cout << msgHeader << "File loaded.\n"; }
	else if(code_ == "REWIND_FILE"){  }
	else{ std::cout << msgHeader << "Unknown notification code '" << code_ << "'!\n"; }
}

/** Return a pointer to the Unpacker object to use for data unpacking.
  * If no object has been initialized, create a new one.
  * \return Pointer to an Unpacker object.
  */
Unpacker *readerScanner::GetCore(){
	if(!core){ core = (Unpacker*)(new readerUnpacker()); }
	return core;
}

/** Add a channel event to the deque of events to send to the processors.
  * This method should only be called from readerUnpacker::ProcessRawEvent().
  * \param[in]  event_ The raw XiaData to add to the channel event deque.
  * \return False.
  */
bool readerScanner::AddEvent(XiaData *event_){
	if(!event_){ return false; }

	ChannelEvent* channelevent_ = new ChannelEvent(event_);

		Trigger_Time = (unsigned long long)channelevent_->time;
		Filter_Energy = channelevent_->energy;
		Event_Time_Lo = channelevent_->eventTimeLo;
		Event_Time_Hi = channelevent_->eventTimeHi;
		Crate = channelevent_->crateNum;
		Slot = channelevent_->slotNum;
		Module = channelevent_->modNum;
		Channel = channelevent_->chanNum;
		CFD_Time = channelevent_->cfdTime;
		Trace_Length = channelevent_->traceLength;
		// adc_Trace;
		num_Qdcs = channelevent_->numQdcs;
		// qdc_Value;
		Virtual = channelevent_->virtualChannel;
		Pileup = channelevent_->pileupBit;
		Saturated = channelevent_->saturatedBit;
		CFD_Force = channelevent_->cfdForceTrig;
		CFD_Trig = channelevent_->cfdTrigSource;
		channelEventTree->Fill();

		// if(channelevent_->traceLength != 0 && showTrace){
		// 	int numLine = 0;
		// 	std::cout << " Trace:\n  ";
		// 	for(size_t i = 0; i < channelevent_->traceLength; i++){
		// 		adc_Trace=channelevent_->adcTrace[i];
		// 		// std::cout << channelevent_->adcTrace[i] << "\t";
		// 		if(++numLine % 10 == 0) std::cout << "\n  ";
		// 	}
		// 	std::cout << std::endl;
		// }


	eventsRead++;
	delete currentEvent;
	currentEvent = channelevent_;

	return true;
}

/** Process all channel events read in from the rawEvent.
  * This method should only be called from readerUnpacker::ProcessRawEvent().
  * \return True.
  */
bool readerScanner::ProcessEvents(){
	// Process all of the events added so far.
	// Do some initialization.

	return true;
}

int main(int argc, char *argv[]){


	TApplication *rootapp = new TApplication("rootapp", 0, NULL);

	// This is done to keep the compiler from complaining about
	// unused TApplication variable.
	rootapp->SetBit(0, false);

	// Define a new unpacker object.
	readerScanner scanner;

	// Set the output message prefix.
	scanner.SetProgramName(std::string(PROG_NAME));

	// Initialize the scanner.
	if(!scanner.Setup(argc, argv))
		return 1;

	// Run the main loop.
	int retval = scanner.Execute();

	scanner.Close();

	return retval;
}
