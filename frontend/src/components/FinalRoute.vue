<template>
  <div class="final-route">
    <h2>Final Route</h2>
    <div v-if="state == 'calculate'" class="loading">
      <p>Loading...</p>
    </div>
    <div v-if="state == 'ready'" class="path">
      <p>
        The route takes
        <span v-if="hours !== 0">{{ hours }} hours, </span>
        <span v-if="minutes !== 0">{{ minutes }} minutes, </span>
        <span v-if="hours !== 0 || minutes !== 0">and</span>
        {{ seconds }} seconds.
      </p>

      <destination-item
        :key="dest.id"
        v-for="dest in finalRoute"
        :destination="dest.name"
        :options="dest.locations"
      />
    </div>
    <button @click="returnToHome">Return</button>
  </div>
</template>

<script>
import config from "../../config.json";
import axios from "axios";
import DestinationItem from "./DestinationItem.vue";

export default {
  name: "FinalRoute",

  components: {
    DestinationItem,
  },

  data() {
    return {
      startingPosition: {},
      endingPosition: {},
      destinations: [],
      finalRoute: [],
      hours: 0,
      minutes: 0,
      seconds: 0,
      state: "calculate",
      places: [],
      placeMarkers: [],
      placeMarkerPositions: [],
      path: Object,
    };
  },

  methods: {
    getApiKey() {
      return config["API_KEY"];
    },
    returnToHome() {
      this.$emit("update:removeMarkers", {
        markers: this.placeMarkers,
        positionObject: this.placeMarkerPositions,
      });
      this.$emit("update:removeLine", this.path);
      this.$emit("update:reset");
    },
    updateFitBounds() {
      this.$emit("update:fitBounds", {
        startingMarkerPosition: [],
        endingMarkerPosition: [],
        destinationMarkerPositions: this.placeMarkerPositions,
        newMarkerPositions: [],
      });
    },
    connectMarkers() {
      const pathCoordinates = this.places.map(
        (marker) => new window.google.maps.LatLng(marker.lat, marker.lng)
      );

      this.path = new window.google.maps.Polyline({
        path: pathCoordinates,
        geodesic: true,
        strokeColor: "#FF0000",
        strokeOpacity: 1.0,
        strokeWeight: 2,
      });

      this.$emit("update:setLine", this.path);
    },
    calculate() {
      let node = [this.startingPosition];
      let dest_string =
        this.startingPosition.latitude +
        "," +
        this.startingPosition.longitude +
        "%";
      let group_count = 1 + ",";
      for (let i = 0; i < this.destinations.length; i++) {
        const dest = this.destinations[i];
        for (let j = 0; j < dest.locations.length; j++) {
          const location = dest.locations[j];
          dest_string += location.lat + "," + location.lng + "%";
          node.push(location);
        }
        group_count += dest.locations.length + ",";
      }
      dest_string +=
        this.endingPosition.latitude + "," + this.endingPosition.longitude;
      group_count += "1";
      node.push(this.endingPosition);

      const key =
        "/api/calculate?destinations=" +
        dest_string +
        "&group_count=" +
        group_count +
        "&api_key=" +
        this.getApiKey();
      axios.get(key).then((response) => {
        this.seconds = response.data.seconds;
        if (this.seconds > 60) {
          this.minutes = Math.floor(this.seconds / 60);
          this.seconds = this.seconds % 60;
        }
        if (this.minutes > 60) {
          this.hours = Math.floor(this.minutes / 60);
          this.minutes = this.minutes % 60;
        }
        const route = response.data.route;
        this.finalRoute.push({
          id: 0,
          name: "Destination 1:",
          locations: [this.startingPosition],
        });
        this.places.push({
          name: this.startingPosition.name,
          lat: this.startingPosition.latitude,
          lng: this.startingPosition.longitude,
        });
        for (let i = 1; i < route.length - 1; i++) {
          this.finalRoute.push({
            id: i,
            name: "Destination " + (i + 1) + ":",
            locations: [node[route[i]]],
          });
          this.places.push(node[route[i]]);
        }
        this.finalRoute.push({
          id: route.length - 1,
          name: "Destination " + route.length + ":",
          locations: [this.endingPosition],
        });
        this.places.push({
          name: this.endingPosition.name,
          lat: this.endingPosition.latitude,
          lng: this.endingPosition.longitude,
        });
        this.state = "ready";
        this.$emit("update:addMarkers", {
          places: this.places,
          markerObject: this.placeMarkers,
          positionsObject: this.placeMarkerPositions,
        });
        this.updateFitBounds();
        this.connectMarkers();
        return;
      });
    },
  },
};
</script>

<style lang="scss">
@import "./FinalRoute.scss";
</style>
