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
        <div class="endpoints">
          <input-field
            ref="startingLocRef"
            :title="'Starting Location'"
            :content="startingLocation"
            :isSearch="true"
            :canSearch="true"
            @update:content="updateValue($event, 'startingLocation')"
            @update:selected="updateSelected"
          />
          <input-field
            ref="endingLocRef"
            :title="'Ending Location'"
            :content="endingLocation"
            :isSearch="true"
            :canSearch="true"
            @update:content="updateValue($event, 'endingLocation')"
            @update:selected="updateSelected"
          />
        </div>
        <div class="destinations">
          <h2>Destinations</h2>
          <destination-item
            :key="dest.id"
            v-for="dest in destinations"
            :destination="dest.name"
            :options="dest.locations"
          />
        </div>
        <div class="add">
          <h2>Add Destination</h2>
          <div class="add-input">
            <input-field
              ref="destinationNameRef"
              :title="''"
              :content="destinationName"
              :isSearch="false"
              :canSearch="true"
              @update:content="updateValue($event, 'destinationName')"
            />
            <input-field
              ref="destinationSearchRef"
              :title="''"
              :content="destinationSearch"
              :isSearch="true"
              :canSearch="false"
              :error="'Please select starting and ending location first.'"
              @update:content="updateValue($event, 'destinationSearch')"
            />
          </div>
          <div class="add-bottom">
            <destination-item
              v-if="newDestination.name"
              :destination="newDestination.name"
              :options="newDestination.locations"
              :key="newDestination.id"
            />
            <div class="add-buttons">
              <button @click="addDestination" class="add-button">Add</button>
              <button @click="cancelDestination" class="add-button">
                Cancel
              </button>
            </div>
          </div>
        </div>

        <div class="buttons">
          <button @click="clearClick" style="margin-right: 20px">Clear</button>
          <button @click="calculateClick">Calculate</button>
        </div>
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
import InputField from "./InputField.vue";
import DestinationItem from "./DestinationItem.vue";
import config from "../../config.json";
import { haversineDistance, calculateMiddlePoint } from "./utils.js";

export default {
  name: "HomePage",

  components: {
    InputField,
    DestinationItem,
  },

  data() {
    return {
      startingLocation: "Search",
      endingLocation: "Search",
      destinationName: "Destination Name",
      destinationSearch: "Search Potential Locations",
      startingMarker: [],
      endingMarker: [],
      destinationMarkers: [],
      markerPositions: [],
      map: Object,
      destinations: [
        {
          id: 0,
          name: "Destination 1",
          numLocs: 3,
          locations: [
            { id: 0, name: "Potential Location 1" },
            { id: 1, name: "Potential Location 2" },
            { id: 2, name: "Potential Location 3" },
          ],
        },
        {
          id: 1,
          name: "Destination 2",
          numLocs: 1,
          locations: [{ name: "Potential Location 1" }],
        },
      ],
      newDestination: {
        name: "Selected:",
        numLocs: 3,
        locations: [
          { id: 0, name: "Potential Location 1" },
          { id: 1, name: "Potential Location 2" },
          { id: 2, name: "Potential Location 3" },
        ],
      },
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
        center: { lat: -34.397, lng: 150.644 },
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
    addMarkers(places, markerObject) {
      places.forEach((placeInfo) => {
        const position = { lat: placeInfo.lat, lng: placeInfo.lng };
        const marker = new window.google.maps.Marker({
          position: position,
          map: this.map,
          title: placeInfo.name,
        });
        markerObject.push({ marker: marker });
        this.markerPositions.push(position);
      });
      const bounds = new window.google.maps.LatLngBounds();
      this.markerPositions.forEach((position) => {
        bounds.extend(
          new window.google.maps.LatLng(position.lat, position.lng)
        );
      });
      this.map.fitBounds(bounds);
    },
    removeMarkers(markers) {
      markers.forEach((marker) => {
        marker.marker.setVisible(false);
        marker.marker.setMap(null);
        marker.marker = null;
      });
      markers.length = 0;
    },
    calculateClick() {},
    clearClick() {
      this.$refs.startingLocRef.clear();
      this.$refs.endingLocRef.clear();
      this.$refs.destinationNameRef.clear();
      this.$refs.destinationSearchRef.clear();
    },
    addDestination() {
      if (Object.keys(this.newDestination).length === 0) return;
      this.destinations.push({
        id: this.destinations.length,
        name: this.destinationName,
        numLocs: this.newDestination.numLocs,
        locations: this.newDestination.locations,
      });
      this.cancelDestination();
    },
    cancelDestination() {
      this.$refs.destinationNameRef.clear();
      this.$refs.destinationSearchRef.clear();
      this.$refs.destinationNameRef.contentVal = "Destination Name";
      this.$refs.destinationSearchRef.contentVal = "Search Potential Locations";
      this.newDestination = {};
    },
    updateSelected() {
      if (
        this.$refs.startingLocRef.placeSelected &&
        this.startingMarker.length == 0
      ) {
        this.addMarkers(
          [
            {
              name: this.$refs.startingLocRef.selectedPlaces[0].name,
              lat: this.$refs.startingLocRef.selectedPlaces[0].latitude,
              lng: this.$refs.startingLocRef.selectedPlaces[0].longitude,
            },
          ],
          this.startingMarker
        );
      } else if (
        !this.$refs.startingLocRef.placeSelected &&
        this.startingMarker.length != 0
      ) {
        this.removeMarkers(this.startingMarker);
      }
      if (
        this.$refs.endingLocRef.placeSelected &&
        this.endingMarker.length == 0
      ) {
        this.addMarkers(
          [
            {
              name: this.$refs.endingLocRef.selectedPlaces[0].name,
              lat: this.$refs.endingLocRef.selectedPlaces[0].latitude,
              lng: this.$refs.endingLocRef.selectedPlaces[0].longitude,
            },
          ],
          this.endingMarker
        );
      } else if (
        !this.$refs.endingLocRef.placeSelected &&
        this.endingMarker.length != 0
      ) {
        this.removeMarkers(this.endingMarker);
      }
      if (
        this.$refs.startingLocRef.placeSelected &&
        this.$refs.endingLocRef.placeSelected
      ) {
        this.$refs.destinationSearchRef.canSearchVal = true;
        this.$refs.destinationSearchRef.errorVal = "";
        const startLat = this.$refs.startingLocRef.selectedPlaces[0].latitude;
        const startLng = this.$refs.startingLocRef.selectedPlaces[0].longitude;
        const endLat = this.$refs.endingLocRef.selectedPlaces[0].latitude;
        const endLng = this.$refs.endingLocRef.selectedPlaces[0].longitude;
        const distance = haversineDistance(startLat, startLng, endLat, endLng);
        const midpoint = calculateMiddlePoint(
          startLat,
          startLng,
          endLat,
          endLng
        );

        let radius = 100000;
        if (distance != 0) {
          radius = distance * 2;
        }
        this.$refs.destinationSearchRef.midpoint = midpoint;
        this.$refs.destinationSearchRef.radius = radius;
      } else {
        this.$refs.destinationSearchRef.canSearchVal = false;
        this.$refs.destinationSearchRef.errorVal =
          "Please select starting and ending location first.";
      }
    },

    updateValue(event, topic) {
      if (topic === "startingLocation") {
        this.startingLocation = event;
      } else if (topic === "endingLocation") {
        this.endingLocation = event;
      } else if (topic === "destinationName") {
        this.destinationName = event;
      } else if (topic === "destinationSearch") {
        this.destinationSearch = event;
      }
    },
  },
};
</script>

<style lang="scss">
@import "./HomePage.scss";
</style>
