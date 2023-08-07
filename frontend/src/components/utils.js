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
