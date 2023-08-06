<template>
  <div class="place-dropdown" v-show="isOpen">
    <!-- Add your dropdown list content here -->
  </div>
</template>

<script>
import config from "../../config.json";

export default {
  name: "PlaceDropdown",
  props: {
    searchText: String,
  },
  data() {
    return {
      isOpen: false,
      places: [],
    };
  },
  methods: {
    getApiKey() {
      return config["API_KEY"];
    },
    searchPlaces(input) {
      console.log("searchPlaces: " + input);
      const url = `https://maps.googleapis.com/maps/api/place/textsearch/json?query=${input}&key=${this.getApiKey()}`;
      fetch(url)
        .then((response) => response.json())
        .then((data) => {
          const results = data.results;
          const resultList = document.getElementById("results");

          resultList.innerHTML = ""; // Clear previous results

          results.forEach((result) => {
            console.log(result);
          });
          /*
          results.forEach((result) => {
            const li = document.createElement("li");
            li.textContent = result.name;
            resultList.appendChild(li);
          });
          */
        })
        .catch((error) => {
          console.error("Error fetching data:", error);
        });
    },
    selectPlace(place) {
      // Handle the selection of a place from the dropdown list.
      // You can emit an event to pass the selected place to the parent component (InputField).
      this.$emit("selectPlace", place);
    },
  },
};
</script>

<style lang="scss">
/* Add your styles for the dropdown list here */
</style>
