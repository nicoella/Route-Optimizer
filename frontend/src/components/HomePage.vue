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
            @update:content="updateValue($event, 'startingLocation')"
          />
          <input-field
            ref="endingLocRef"
            :title="'Ending Location'"
            :content="endingLocation"
            :isSearch="true"
            @update:content="updateValue($event, 'endingLocation')"
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
              @update:content="updateValue($event, 'destinationName')"
            />
            <input-field
              ref="destinationSearchRef"
              :title="''"
              :content="destinationSearch"
              :isSearch="true"
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
          <div ref="map" style="width: 100%; height: 100%"></div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import InputField from "./InputField.vue";
import DestinationItem from "./DestinationItem.vue";
import config from "../config.json";

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

      new window.google.maps.Map(mapContainer, {
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
        script.src = `https://maps.googleapis.com/maps/api/js?key=${this.getApiKey()}&callback=initMap`;
        script.defer = true;
        script.async = false;
        script.onerror = () => {
          console.error("Failed to load Google Maps API.");
        };
        document.head.appendChild(script);
      }
    },
    calculateClick() {},
    clearClick() {
      this.$refs.startingLocRef.clear();
      this.$refs.endingLocRef.clear();
      this.$refs.destinationNameRef.clear();
      this.$refs.destinationSearchRef.clear();
    },
    addDestination() {
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
      this.newDestination = [];
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
