<template>
  <div class="home_page">
    <h1>ROUTE OPTIMIZER</h1>
    <p>
      A calculator that determines the shortest route that visits one location
      for each route destination.
    </p>
    <br />

    <div class="container">
      <div class="left">
        <div class="endpoints">
          <search-field :title="'Starting Location'" :content="'Search'" />
          <search-field :title="'Ending Location'" :content="'Search'" />
        </div>
        <div class="destinations">
          <h2>Destinations</h2>
          <destination-item
            :destination="'Destination 1'"
            :options="[
              { id: 1, name: 'Potential Location 1' },
              { id: 2, name: 'Potential Location 2' },
              { id: 3, name: 'Potential Location 3' },
            ]"
          />
        </div>
        <div class="add">
          <search-field :title="'Add Destination'" :content="'Search'" />
        </div>
      </div>
      <div class="right">
        <div ref="mapContainer" class="map-container">
          <div ref="map" style="width: 100%; height: 100%"></div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import SearchField from "./SearchField.vue";
import DestinationItem from "./DestinationItem.vue";
import config from "../config.json";

export default {
  name: "HomePage",
  components: {
    SearchField,
    DestinationItem,
  },
  props: {
    msg: String,
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
  },
};
</script>

<style>
.home_page {
  padding-top: 20px;
  padding-left: 10vw;
  padding-right: 10vw;
}

h1 {
  color: var(--grey-dark);
  font-size: 20px;
}

h2 {
  color: var(--grey-light);
  font-family: monospace;
  font-size: 10px;
  font-weight: 300;
  text-align: left;
}

p {
  color: var(--grey-med);
  font-size: 14px;
  font-weight: 400;
}

.container {
  display: flex;
}

.endpoints {
  display: flex;
  width: 100%;
}

.destinations {
  padding-top: 20px;
  padding-bottom: 20px;
}

.add {
  width: 50%;
}

.left {
  width: 49%;
}

.right {
  width: 50%;
  height: 400px;
  margin-left: auto;
  background-color: blue;
}

.map-container {
  width: 100%;
  height: 100%;
}
</style>
