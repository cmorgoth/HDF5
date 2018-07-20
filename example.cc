/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.     *
 * Copyright by the Board of Trustees of the University of Illinois.     *
 * All rights reserved.     *
 *                                                                     *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF5/releases.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 *  This example illustrates how to write to and read from an existing
 *  dataset. It is used in the HDF5 Tutorial.
 */

#include <iostream>
#include <string>
#include "H5Cpp.h"
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>

using namespace H5;

//const H5std_string FILE_NAME("/Users/cmorgoth/Dropbox/3BA100_LF1-20K_80GS_6BW-12BW_test-seg_1550nm_P233_19.4uA_1of3.h5");
const H5std_string FILE_NAME("/Users/cmorgoth/Downloads/2AB120_LF3-20K_80GS_6BW-12BW_segmented_532nm_TE_33.4uA_3of3.h5");
const H5std_string DATASET_NAME("Waveforms");
const int DIM0 = 4;               // dataset dimensions
const int DIM1 = 6;


const unsigned int NUM_CHANNELS = 2;
const unsigned int NUM_TIMES = 1;
const unsigned int NUM_SAMPLES = 42;
//const unsigned int DAC_RESOLUTION;

int main (void)
{

  // Data initialization.

  int i, j;
  int data[DIM0][DIM1];    // buffer for data to write

  for (j = 0; j < DIM0; j++)
    for (i = 0; i < DIM1; i++)
      data[j][i] = i * 6 + j + 1;

  // Try block to detect exceptions raised by any of the calls inside it
    try
      {
	// Turn off the auto-printing when failure occurs so that we can
	// handle the errors appropriately
	//Exception::dontPrint();

  TTree* tree = new TTree("tree", "Waveforms from Keysight Scope");
  int event;
  int mydata2[42];
  int mydata4[42];
  float channel[NUM_CHANNELS][NUM_SAMPLES];
  //float ch4[42];
  float time[NUM_TIMES][NUM_SAMPLES];
  tree->Branch("event", &event, "event/I");
  //tree->Branch ("ch2", ch2, "ch2[42]/F");
  //tree->Branch ("ch4", ch4, "ch4[42]/F");
  tree->Branch("channel", &(channel[0][0]), Form("channel[%d][%d]/F", NUM_CHANNELS, NUM_SAMPLES));
  tree->Branch("time", &(time[0][0]), Form("time[%d][%d]/F", NUM_TIMES, NUM_SAMPLES));
  //tree->Branch ("time", time, "time[42]/F");
	// Open an existing file and dataset.
	H5File file(FILE_NAME, H5F_ACC_RDWR);
  Group  group = file.openGroup("Waveforms");
  Group  sgroup2 = group.openGroup("Channel 2");
  Group  sgroup4 = group.openGroup("Channel 4");


  for( int i = 0; i < sgroup2.getNumObjs(); i++ )
  //for( int i = 0; i < 10; i++ )
  {
    if ( i % 5000 == 0 ) std::cout << "[INFO]: event " << i << std::endl;
    //std::cout << sgroup.getObjnameByIdx(i) << std::endl;
    DataSet dataset2 = sgroup2.openDataSet(sgroup2.getObjnameByIdx(i));
    DataSet dataset4 = sgroup4.openDataSet(sgroup4.getObjnameByIdx(i));
    /*
    * Get dataspace of the dataset.
    */
    DataSpace dataspace2 = dataset2.getSpace();
    /*
    * Get the dimension size of each dimension in the dataspace and
    * display them.
    */
    hsize_t dims_out[2] = {0,0};
    int ndims = dataspace2.getSimpleExtentDims( dims_out, NULL);
    /*
    std::cout << "rank " << rank << ", dimensions " <<
    (unsigned long)(dims_out[0]) << " x " <<
    (unsigned long)(dims_out[1]) << std::endl;
    */
    dataset2.read(mydata2,PredType::NATIVE_INT);
    dataset4.read(mydata4,PredType::NATIVE_INT);
    for( int i = 0; i < dims_out[0]; i++)
    {
      channel[0][i] = float(mydata2[i]);
      channel[1][i] = float(mydata4[i]);
      //std::cout << ch2[i] << std::endl;
      time[0][i] = float(i)*12.5;//ps
    }
    tree->Fill();
    event++;
  }

  //TGraph* gr2 = new TGraph(42, time, ch2);
  //TGraph* gr4 = new TGraph(42, time, ch4);
  TFile* fout = new TFile("test_532nm_v3.root", "RECREATE");
  //gr2->Write("wf2");
  //gr4->Write("wf4");
  tree->Write();
  fout->Close();
	//DataSet dataset = sgroup.openDataSet(DATASET_NAME);

	// Write the data to the dataset using default memory space, file
	// space, and transfer properties.
	//dataset.write(data, PredType::NATIVE_INT);

      }  // end of try block

    // catch failure caused by the H5File operations
    catch(FileIException error)
      {
	//error.printError();
	return -1;
      }

    // catch failure caused by the DataSet operations
    catch(DataSetIException error)
      {
	//error.printError();
	return -1;
      }

    return 0;  // successfully terminated
}
