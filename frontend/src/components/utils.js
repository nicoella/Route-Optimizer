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

export function distToSegment(x, y, x1, y1, x2, y2) {
  var A = x - x1;
  var B = y - y1;
  var C = x2 - x1;
  var D = y2 - y1;

  var dot = A * C + B * D;
  var len_sq = C * C + D * D;
  var param = -1;
  if (len_sq != 0) param = dot / len_sq;

  var xx, yy;

  var dx;
  var dy;

  if (param < 0) {
    xx = x1;
    yy = y1;
    dx = x1 - x;
    dy = y1 - y;
  } else if (param > 1) {
    xx = x2;
    yy = y2;
    dx = x2 - xx;
    dy = y2 - yy;
  } else {
    xx = x1 + param * C;
    yy = y1 + param * D;
    dx = x - x;
    dy = y - y;
  }

  return Math.sqrt(dx * dx + dy * dy);
}
