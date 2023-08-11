<template>
  <div
    class="place-dropdown"
    v-show="is_open"
    v-if="places.length > 0"
    @click.stop=""
  >
    <ul>
      <li
        v-for="place in places.slice(0, 20)"
        :key="place.id"
        @click="select_place(place.id)"
        :class="{ selected: place.selected }"
      >
        <div class="name">{{ place.name }}</div>
        <div class="address">
          {{ place.address }}
        </div>
      </li>
    </ul>
  </div>
</template>

<script>
/* eslint-disable */
import config from "../../config.json";
import axios from "axios";
import { distToSegment } from "./utils.js";

export default {
  name: "PlaceDropdown",
  props: {
    type: String,
  },
  data() {
    return {
      is_open: false,
      places: [],
      placeSelected: false,
      selectedPlaces: [],
      cur_query: "",
    };
  },
  methods: {
    getApiKey() {
      return config["API_KEY"];
    },
    searchPlaces(input) {
      if (
        this.cur_query == input.query ||
        input.query == "Search Potential Locations" ||
        input.query == "Search"
      )
        return;
      this.placeSelected = false;
      this.selectedPlaces = [];
      this.cur_query = input.query;
      this.$emit("update:selected", 0);
      let key = "";
      if (this.type == "find_place") {
        key =
          "/api/find_place?query=" +
          input.query +
          "&api_key=" +
          this.getApiKey();
        axios
          .get(key)
          .then((response) => {
            if (response.data.status == "OK") {
              this.is_open = true;
              this.places = [];
              for (let candidate of response.data.candidates) {
                this.places.push({
                  id: this.places.length,
                  name: candidate.name,
                  address: candidate.formatted_address,
                  latitude: candidate.geometry.location.lat,
                  longitude: candidate.geometry.location.lng,
                  selected: false,
                });
              }
            } else {
              this.places = [];
            }
          })
          .catch((error) => {
            console.error(error);
          });
      } else if (this.type == "text_search") {
        key =
          "/api/text_search?query=" +
          input.query +
          "&latitude=" +
          input.midpoint.latitude +
          "&longitude=" +
          input.midpoint.longitude +
          "&radius=" +
          input.radius +
          "&api_key=" +
          this.getApiKey();
        axios
          .get(key)
          .then((response) => {
            if (response.data.status == "OK") {
              this.is_open = true;
              this.places = [];

              for (let result of response.data.results) {
                this.places.push({
                  id: this.places.length,
                  distance: distToSegment(
                    input.startingPosition.latitude,
                    input.startingPosition.longitude,
                    input.endingPosition.latitude,
                    input.endingPosition.longitude,
                    result.geometry.location.lat,
                    result.geometry.location.lng
                  ),
                  name: result.name,
                  address: result.formatted_address,
                  latitude: result.geometry.location.lat,
                  longitude: result.geometry.location.lng,
                  selected: false,
                });
              }
              this.places.sort((a, b) => a.distance - b.distance);
              this.places.forEach((place, index) => {
                place.id = index;
              });
            } else {
              this.places = [];
            }
          })
          .catch((error) => {
            console.error(error);
          });
      }
    },
    select_place(id) {
      this.places[id].selected = !this.places[id].selected;
      if (this.type == "find_place") {
        if (this.places[id].selected) {
          this.placeSelected = true;
          this.selectedPlaces.push({
            name: this.places[id].name,
            address: this.places[id].address,
            latitude: this.places[id].latitude,
            longitude: this.places[id].longitude,
          });
        } else {
          this.placeSelected = false;
          this.selectedPlaces = [];
        }
        this.cur_query = this.places[id].name;
        this.$emit("update:content", id);
      } else if (this.type == "text_search") {
        if (this.places[id].selected) {
          this.placeSelected = true;
          this.selectedPlaces.push({
            objectId: id,
            name: this.places[id].name,
            address: this.places[id].address,
            latitude: this.places[id].latitude,
            longitude: this.places[id].longitude,
          });
        } else {
          this.selectedPlaces.splice(
            this.selectedPlaces.findIndex((place) => place.objectId === id),
            1
          );
        }
        this.$emit("update:selected", id);
      }
    },
  },
};
</script>

<style lang="scss">
@import "./PlaceDropdown.scss";
</style>
