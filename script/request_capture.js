function handleCapture(canvas) {
  console.log("Handling capture!");
  return canvas.toDataURL('png');
};

function requestCapture(seed) {
  console.log("Requesting capture");
  if (window.hasOwnProperty('SolarNWInterface')) {
    window.SolarNWInterface.requestCapture({ seed: seed }, handleCapture);
    return "Done!";
  } else {
    return "SolarNWInterface not found!";
  }
};

requestCapture(5);
