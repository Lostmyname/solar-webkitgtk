function requestCapture(seed) {
  console.log("Requesting capture");
  if (window.hasOwnProperty('SolarNWInterface')) {
    window.SolarNWInterface.requestCaptureSync({ seed: seed });
    return "Done!";
  } else {
    return "SolarNWInterface not found!";
  }
};

var thing = requestCapture(5);

thing;
