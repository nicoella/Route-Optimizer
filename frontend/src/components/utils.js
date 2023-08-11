/* eslint-disable */
function degreesToRadians(degrees) {
  return degrees * (Math.PI / 180);
}

function radiansToDegrees(radians) {
  return radians * (180 / Math.PI);
}

export function calculateMiddlePoint(lat1, lon1, lat2, lon2) {
  lat1 = degreesToRadians(lat1);
  lon1 = degreesToRadians(lon1);
  lat2 = degreesToRadians(lat2);
  lon2 = degreesToRadians(lon2);

  const avgLat = (lat1 + lat2) / 2;
  const avgLon = (lon1 + lon2) / 2;

  const avgLatDeg = radiansToDegrees(avgLat);
  const avgLonDeg = radiansToDegrees(avgLon);

  return { latitude: avgLatDeg, longitude: avgLonDeg };
}

export function haversineDistance(lat1, lon1, lat2, lon2) {
  const R = 6371.0;

  lat1 = degreesToRadians(lat1);
  lon1 = degreesToRadians(lon1);
  lat2 = degreesToRadians(lat2);
  lon2 = degreesToRadians(lon2);

  const dlat = lat2 - lat1;
  const dlon = lon2 - lon1;

  // Haversine formula
  const a =
    Math.sin(dlat / 2) ** 2 +
    Math.cos(lat1) * Math.cos(lat2) * Math.sin(dlon / 2) ** 2;
  const c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));

  const distance = R * c * 1000;
  return distance; // meters
}

function signedDistanceOnEquator(lat, lat1) {
  const R = 6371.0 * 1000; // Earth's radius in meters

  lat = degreesToRadians(lat);
  lat1 = degreesToRadians(lat1);

  const dlat = lat - lat1;

  const signedDistance = R * dlat;
  return signedDistance;
}

function addMetersToLatitude(lat, meters) {
  const earthRadius = 6371000;
  const radiansPerMeter = 1 / earthRadius;

  const latChangeRadians = meters * radiansPerMeter;

  const newLat = lat + latChangeRadians * (180 / Math.PI);

  return newLat;
}

export function distToSegment(lat, lon, lat1, lon1, lat2, lon2) {
  var A = signedDistanceOnEquator(lat, lat1);
  var B = signedDistanceOnEquator(lon, lon1);
  var C = signedDistanceOnEquator(lat2, lat1);
  var D = signedDistanceOnEquator(lon2, lon1);

  var dot = A * C + B * D;
  var len_sq = C * C + D * D;
  var param = -1;
  if (len_sq != 0)
    //in case of 0 length line
    param = dot / len_sq;

  var xx, yy;

  if (param < 0) {
    xx = lat1;
    yy = lon1;
  } else if (param > 1) {
    xx = lat2;
    yy = lon2;
  } else {
    xx = addMetersToLatitude(lat1, param * C);
    yy = addMetersToLatitude(lon1, param * D);
  }

  return haversineDistance(lat, lon, xx, yy);
}
