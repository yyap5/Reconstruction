#include "ParameterMessenger.hxx"

void ParameterMessenger::LoadFile(const std::string& filename, TrackReconstructor& reconstructor, Recon3D& reconstructor_3D){

  std::ifstream file(filename);

  if(!file.is_open()){
    throw std::runtime_error("cannot open parameter file: " + filename);
  }

  std::string line;
  int line_number = 0;

  while(std::getline(file, line)){
    ++line_number;

    //Remove comment
    const auto comment_pos = line.find("#");
    if(comment_pos != std::string::npos){
      line.erase(comment_pos);
    }

    std::istringstream iss(line);
    std::string command;
    Float_t value;

    //skip if there are wrong things in command position
    if(!(iss >> command)){continue;}

    //skip if there are wrong things in value position
    if(!(iss >> value)){continue;}

    //skip if there are something at the end
    std::string extra;
    if(iss >> extra){continue;}

    if(command == "TrackReconstructor/MaxSlopeChange"){
      reconstructor.SetMaxSlopeChange(value);
    }
    else if(command == "TrackReconstructor/MaxSlopeChangeAdjacent"){
      reconstructor.SetMaxSlopeChangeAdjacent(value);
    }
    else if(command == "TrackReconstructor/MaxLength"){
      reconstructor.SetMaxLength(value);
    }
    else if(command == "Recon3D/MatchingLayerNumber"){
      reconstructor_3D.SetMatchingLayerNumber(static_cast<Int_t>(value));
    }
    else if(command == "Recon3D/MatchingOverlapThreshold"){
      reconstructor_3D.SetMatchingOverlapThreshold(static_cast<Int_t>(value));
    }
    else{
      std::cout << "Unknown command : " << command << std::endl;
    }
  }

}
