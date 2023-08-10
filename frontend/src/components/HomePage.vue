<template>
  <div class="home_page">
    <h1>ROUTE OPTIMIZER</h1>
    <div class="intro">
      <p>
        A calculator to find the shortest route that visits one location for
        each destination.
      </p>
      <p>
        Instructions: Enter a starting and ending location. Enter one
        destination for each category to stop at, and select potential locations
        for the destination.
      </p>
    </div>

    <div class="container">
      <div class="left">
        <manual-input
          v-if="state == 'ManualInput'"
          ref="manualInputRef"
          @update:fitBounds="updateFitBounds"
          @update:addMarkers="addMarkers"
          @update:removeMarkers="removeMarkers"
          @update:destinations="updateDestinations"
        />
        <final-route
          v-if="state == 'FinalRoute'"
          ref="finalRouteRef"
          @update:fitBounds="updateFitBounds"
          @update:addMarkers="addMarkers"
          @update:reset="reset"
          @update:setLine="setLine"
          @update:removeMarkers="removeMarkers"
          @update:removeLine="removeLine"
        />
      </div>
      <div class="right">
        <h2>Please reload the page to load the map.</h2>
        <div ref="mapContainer" class="map-container">
          <div
            id="map_canvas"
            ref="map"
            style="width: 100%; height: 100%"
          ></div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
/* eslint-disable */
import ManualInput from "./ManualInput.vue";
import FinalRoute from "./FinalRoute.vue";
import config from "../../config.json";

export default {
  name: "HomePage",

  components: {
    ManualInput,
    FinalRoute,
  },

  data() {
    return {
      map: Object,
      state: "ManualInput",
    };
  },

  mounted() {
    this.loadGoogleMaps();
    window.initMap = this.initMap;
  },

  methods: {
    getApiKey() {
      return config["API_KEY"];
    },
    initMap() {
      if (this.mapInitialized) {
        return;
      }
      const mapContainer = this.$refs.mapContainer;

      this.map = new window.google.maps.Map(mapContainer, {
        center: { lat: 0, lng: 0 },
        zoom: 8,
      });

      this.mapInitialized = true;
    },
    loadGoogleMaps() {
      if (
        typeof window.google === "undefined" ||
        typeof window.google.maps === "undefined"
      ) {
        const script = document.createElement("script");
        script.src = `https://maps.googleapis.com/maps/api/js?key=${this.getApiKey()}&callback=initMap&libraries=places`;
        script.defer = true;
        script.async = false;
        script.onerror = () => {
          console.error("Failed to load Google Maps API.");
        };
        document.head.appendChild(script);
      }
    },
    extend(bounds, markerPosition) {
      markerPosition.forEach((position) => {
        bounds.extend(
          new window.google.maps.LatLng(position.lat, position.lng)
        );
      });
    },
    updateFitBounds(event) {
      const startingMarkerPosition = event.startingMarkerPosition;
      const endingMarkerPosition = event.endingMarkerPosition;
      const destinationMarkerPositions = event.destinationMarkerPositions;
      const newMarkerPositions = event.newMarkerPositions;
      const bounds = new window.google.maps.LatLngBounds();
      this.extend(bounds, startingMarkerPosition);
      this.extend(bounds, endingMarkerPosition);
      this.extend(bounds, destinationMarkerPositions);
      this.extend(bounds, newMarkerPositions);
      this.map.fitBounds(bounds);
    },
    addMarkers(event) {
      const places = event.places;
      const markerObject = event.markerObject;
      const positionsObject = event.positionsObject;
      places.forEach((placeInfo) => {
        const position = { lat: placeInfo.lat, lng: placeInfo.lng };
        const marker = new window.google.maps.Marker({
          position: position,
          map: this.map,
          title: placeInfo.name,
        });
        markerObject.push({ marker: marker });
        positionsObject.push(position);
      });
    },
    removeMarkers(event) {
      const markers = event.markers;
      const positionObject = event.positionObject;
      markers.forEach((marker) => {
        if (marker) {
          marker.marker.setVisible(false);
          marker.marker.setMap(null);
          marker.marker = null;
        }
      });
      markers.length = 0;
      positionObject.length = 0;
    },
    setLine(event) {
      event.setMap(this.map);
    },
    removeLine(event) {
      event.setVisible(false);
      event.setMap(null);
      event = null;
    },
    updateDestinations(event) {
      this.state = "FinalRoute";
      this.$nextTick(() => {
        this.$refs.finalRouteRef.startingPosition = event.startingPosition;
        this.$refs.finalRouteRef.endingPosition = event.endingPosition;
        this.$refs.finalRouteRef.destinations = event.destinations;
        this.$refs.finalRouteRef.calculate();
      });
    },
    reset() {
      this.state = "ManualInput";
      this.mapInitialized = false;
      this.initMap();
      this.$nextTick(() => {
        this.$refs.manualInputRef.clearClick();
      });
    },
  },
};
</script>

<style lang="scss">
@import "./HomePage.scss";
</style>
