/// \file RD52FileReader.cc
/*
 *
 * RD52FileReader.cc source template automatically generated by a class generator
 * Creation date : lun. mars 7 2016
 *
 * This file is part of DQM4HEP libraries.
 *
 * DQM4HEP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 *
 * DQM4HEP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DQM4HEP.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author Tom Coates
 * @copyright CNRS , IPNL
 */


// -- std headers 
#include <sstream>

// -- dqm4hep headers
#include <dqm4hep/Event.h>
#include <dqm4hep/EventReader.h>
#include <dqm4hep/StatusCodes.h>
#include <dqm4hep/GenericEvent.h>
#include <dqm4hep/PluginManager.h>
#include <dqm4hep/XmlHelper.h>

namespace dqm4hep {

  namespace core {

    /**
     *  @brief  RD52FileReader class
     *          Read a DREAM RD52 calorimeter .dat datafile and read GenericEvent events from it
     */
    class RD52FileReader : public EventReader {
    public:
      RD52FileReader() = default;
      ~RD52FileReader() override;
      RD52FileReader(const RD52FileReader&) = delete;
      RD52FileReader& operator=(const RD52FileReader&) = delete;

      core::StatusCode open(const std::string &fname) override;
      core::StatusCode skipNEvents(int nEvents) override;
      core::StatusCode runInfo(core::Run &run) override;
      core::StatusCode readNextEvent() override;
      core::StatusCode close() override;
      
      typedef struct {
	uint32_t magicWord;        // Magic word - 0xaabbccdd
	uint32_t headerSize;       // Size in bytes
	uint32_t runNumber;
	uint32_t numberOfEvents;
	uint32_t startTime;
	uint32_t endTime;
      } RunHeader;

      typedef struct {
	uint32_t eventMarker;      // Beginning of event marker - 0xcafecafe
	uint32_t eventHeaderSize;  // In bytes
	uint32_t eventSize;        // In bytes (including header)
	uint32_t eventNumber;
	uint32_t spillNumber;
	uint32_t tsec;             // Seconds in the day from gettimeofday
	uint32_t tusec;            // Microseconds in the day from gettimeofday
      } EventHeader;

      typedef struct {
	uint32_t beginMarker;      // Beginning of subevent marker - 0xacabacab
	uint32_t headerSize;       // In bytes
	uint32_t moduleID;
	uint32_t subeventSize;
      } SubEventHeader;

    protected:
      std::fstream inputFile;

    };
    
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    RD52FileReader::~RD52FileReader() {
      //
    }

    //-------------------------------------------------------------------------------------------------

    StatusCode RD52FileReader::open(const std::string &fname) {
      
      inputFile = fopen(fname.c_str(), "r");

      if (!inputFile.is_open()) {
	dqm_error("Failed to open the file at {0}", fname);
	return STATUS_CODE_FAILURE;
      }

      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------

    StatusCode RD52FileReader::skipNEvents(int nEvents) {

      for (int e=0; e<nEvents; e++) {
	//
	// ...
	//
      }

      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------

    StatusCode RD52FileReader::runInfo(core::Run &run) {

      RunHeader myRunHeader;
      fread(&myRunHeader, sizeof(RunHeader), 1, inputFile);

      run.runNumber(myRunHeader.runNumber);
      run.setStartTime(myRunHeader.startTime);
      run.setEndTime(core::time::asPoint(myRunHeader.endTime));
      run.setParameter("Number of events", myRunHeader.numberOfEvents);
      run.setParameter("Magic word", myRunHeader.magicWord);

      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------

    StatusCode RD52FileReader::readNextEvent() {
      EventPtr pEvent = GenericEvent::make_shared();
      GenericEvent *pGenericEvent = pEvent->getEvent<GenericEvent>();

      //
      // some error checking
      //

      EventHeader myEventHeader;
      fread(&myEventHeader, sizeof(myEventHeader), 1, inputFile);

      SubEventHeader mySubEventHeader; 
      fread(&mySubEventHeader, sizeof(mySubEventHeader), 1, inputFile);

      //
      // some event reading
      //

      onEventRead().emit(pEvent);
      return STATUS_CODE_SUCCESS;
    }

    StatusCode RD52FileReader::close() {
      
      // closing

      fclose(inputFile);
      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------
    
    DQM_PLUGIN_DECL(RD52FileReader, "RD52FileReader");
  }
}