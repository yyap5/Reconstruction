#ifndef PARAMETERMESSENGER_HXX
#define PARAMETERMESSENGER_HXX

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "TrackReconstructor.hxx"
#include "Recon3D.hxx"

class ParameterMessenger{
  public:
    void LoadFile(const std::string& filename, TrackReconstructor& reconstructor, Recon3D& reconstructor_3D);
};


#endif
