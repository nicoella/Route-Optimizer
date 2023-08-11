<template>
  <div class="manual-input">
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
          ref="destinationSearchRef"
          :title="''"
          :content="destinationSearch"
          :isSearch="true"
          :canSearch="false"
          :error="'Please select starting and ending location first.'"
          @update:content="updateValue($event, 'destinationSearch')"
          @update:selected="updateSelectedLocations"
        />
      </div>
      <div class="add-bottom">
        <destination-item
          ref="newDestinationRef"
          v-if="newDestination.name"
          :destination="newDestination.name"
          :options="newDestination.locations"
          :key="newDestination.id"
        />
        <div class="add-buttons">
          <button @click="addDestination" class="add-button">Add</button>
          <button @click="cancelDestination" class="add-button">Cancel</button>
        </div>
      </div>
    </div>

    <div class="buttons">
      <button @click="clearClick" style="margin-right: 20px">Clear</button>
      <button @click="calculateClick">Calculate</button>
    </div>
  </div>
</template>

<script>
import InputField from "./InputField.vue";
import DestinationItem from "./DestinationItem.vue";
import { haversineDistance, calculateMiddlePoint } from "./utils.js";
import config from "../../config.json";

export default {
  name: "ManualInput",

  components: {
    InputField,
    DestinationItem,
  },

  data() {
    return {
      startingLocation: "Search",
      endingLocation: "Search",
      startingPosition: {},
      destinationSearch: "Search Potential Locations",
      endingPosition: {},
      startingMarker: [],
      startingMarkerPosition: [],
      endingMarker: [],
      endingMarkerPosition: [],
      destinationMarkers: [],
      destinationMarkerPositions: [],
      newMarkers: [],
      newMarkerPositions: [],
      destinations: [
        {
          id: 0,
          name: "None",
          locations: [],
        },
      ],
      newDestination: {
        name: "Selected:",
        locations: [{ id: 0, name: "None" }],
      },
    };
  },

  methods: {
    getApiKey() {
      return config["API_KEY"];
    },
    calculateClick() {
      if (
        this.destinations.length == 0 &&
        this.destinations[0].name == "None"
      ) {
        alert("Please enter at least one destination.");
        return;
      }
      this.removeAllMarkers();
      this.$emit("update:destinations", {
        startingPosition: this.startingPosition,
        destinations: this.destinations,
        endingPosition: this.endingPosition,
      });
    },
    clearClick() {
      this.$refs.startingLocRef.clear();
      this.$refs.endingLocRef.clear();
      this.$refs.destinationSearchRef.clear();
      this.$refs.destinationSearchRef.contentVal = "Search Potential Locations";
      this.newDestination = {
        name: "Selected:",
        locations: [{ id: 0, name: "None" }],
      };
      this.destinations = [
        {
          id: 0,
          name: "None",
          locations: [],
        },
      ];
      this.$emit("update:removeMarkers", {
        markers: this.startingMarker,
        positionObject: this.startingMarkerPosition,
      });
      this.$emit("update:removeMarkers", {
        markers: this.endingMarker,
        positionObject: this.endingMarkerPosition,
      });
      this.$emit("update:removeMarkers", {
        markers: this.destinationMarkers,
        positionObject: this.destinationMarkerPositions,
      });
      this.$emit("update:removeMarkers", {
        markers: this.newMarkers,
        positionObject: this.newMarkerPositions,
      });
      this.updateFitBounds();
    },
    updateFitBounds() {
      this.$emit("update:fitBounds", {
        startingMarkerPosition: this.startingMarkerPosition,
        endingMarkerPosition: this.endingMarkerPosition,
        destinationMarkerPositions: this.destinationMarkerPositions,
        newMarkerPositions: this.newMarkerPositions,
      });
    },
    removeAllMarkers() {
      this.$emit("update:removeMarkers", {
        markers: this.startingMarker,
        positionObject: this.startingMarkerPosition,
      });
      this.$emit("update:removeMarkers", {
        markers: this.endingMarker,
        positionObject: this.endingMarkerPosition,
      });
      this.$emit("update:removeMarkers", {
        markers: this.destinationMarkers,
        positionObject: this.destinationMarkerPositions,
      });
      this.$emit("update:removeMarkers", {
        markers: this.newMarkers,
        positionObject: this.newMarkerPositions,
      });
      this.updateFitBounds();
    },
    addDestination() {
      if (this.newDestination.locations[0].name == "None") {
        alert("Please enter a destination name and at least one location.");
        return;
      }
      this.destinations.push({
        id: this.destinations.length,
        name: this.destinationSearch,
        locations: this.newDestination.locations,
      });
      this.$emit("update:addMarkers", {
        places: this.newDestination.locations,
        markerObject: this.destinationMarkers,
        positionsObject: this.destinationMarkerPositions,
      });
      this.updateFitBounds();
      if (this.destinations[0].name == "None") {
        this.destinations.splice(0, 1);
      }
      this.cancelDestination();
    },
    cancelDestination() {
      this.$refs.destinationSearchRef.clear();
      this.newDestination = {
        name: "Selected:",
        locations: [{ id: 0, name: "None" }],
      };
      this.$refs.newDestinationRef.update(
        this.newDestination.name,
        this.newDestination.locations
      );
      this.$emit("update:removeMarkers", {
        markers: this.newMarkers,
        positionObject: this.newMarkerPositions,
      });
      this.updateFitBounds();
    },
    updateSelected() {
      if (
        this.$refs.startingLocRef.placeSelected &&
        this.startingMarker.length == 0
      ) {
        this.startingPosition = this.$refs.startingLocRef.selectedPlaces[0];
        this.$emit("update:addMarkers", {
          places: [
            {
              name: this.$refs.startingLocRef.selectedPlaces[0].name,
              lat: this.$refs.startingLocRef.selectedPlaces[0].latitude,
              lng: this.$refs.startingLocRef.selectedPlaces[0].longitude,
            },
          ],
          markerObject: this.startingMarker,
          positionsObject: this.startingMarkerPosition,
        });
        this.updateFitBounds();
      } else if (
        !this.$refs.startingLocRef.placeSelected &&
        this.startingMarker.length != 0
      ) {
        this.$emit("update:removeMarkers", {
          markers: this.startingMarker,
          positionObject: this.startingMarkerPosition,
        });
        this.updateFitBounds();
      }
      if (
        this.$refs.endingLocRef.placeSelected &&
        this.endingMarker.length == 0
      ) {
        this.endingPosition = this.$refs.endingLocRef.selectedPlaces[0];
        this.$emit("update:addMarkers", {
          places: [
            {
              name: this.$refs.endingLocRef.selectedPlaces[0].name,
              lat: this.$refs.endingLocRef.selectedPlaces[0].latitude,
              lng: this.$refs.endingLocRef.selectedPlaces[0].longitude,
            },
          ],
          markerObject: this.endingMarker,
          positionsObject: this.endingMarkerPosition,
        });
        this.updateFitBounds();
      } else if (
        !this.$refs.endingLocRef.placeSelected &&
        this.endingMarker.length != 0
      ) {
        this.$emit("update:removeMarkers", {
          markers: this.endingMarker,
          positionObject: this.endingMarkerPosition,
        });
        this.updateFitBounds();
      }
      if (
        this.$refs.startingLocRef.placeSelected &&
        this.$refs.endingLocRef.placeSelected
      ) {
        this.$refs.destinationSearchRef.startingPosition =
          this.startingPosition;
        this.$refs.destinationSearchRef.endingPosition = this.endingPosition;
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

        let radius = 50000;
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
    updateSelectedLocations() {
      this.newDestination.name = "Selected:";
      this.newDestination.locations = [];
      this.$refs.destinationSearchRef.selectedPlaces.forEach((location) => {
        this.newDestination.locations.push({
          id: this.newDestination.locations.length,
          name: location.name,
          lat: location.latitude,
          lng: location.longitude,
          address: location.address,
        });
      });
      this.$emit("update:removeMarkers", {
        markers: this.newMarkers,
        positionObject: this.newMarkerPositions,
      });
      this.updateFitBounds();
      this.$emit("update:addMarkers", {
        places: this.newDestination.locations,
        markerObject: this.newMarkers,
        positionsObject: this.newMarkerPositions,
      });
      this.updateFitBounds();
      if (this.newDestination.locations.length == 0) {
        this.newDestination.locations.push({ id: 0, name: "None" });
      }
      this.$refs.newDestinationRef.update(
        this.newDestination.name,
        this.newDestination.locations
      );
    },
    updateValue(event, topic) {
      if (topic === "startingLocation") {
        this.startingLocation = event;
      } else if (topic === "endingLocation") {
        this.endingLocation = event;
      } else if (topic === "destinationSearch") {
        this.destinationSearch = event;
      }
    },
  },
};
</script>

<style lang="scss">
@import "./ManualInput.scss";
</style>
