<template>
  <div class="input-field">
    <h2 v-if="titleVal != ''">{{ titleVal }}</h2>
    <div class="input-bar" v-click-outside="handleContentBlur" tabindex="0">
      <input
        ref="inputField"
        type="text"
        v-model="contentVal"
        @focus="handleContentFocus"
        @input="handleContentChange"
        :disabled="!canSearchVal"
        :style="{
          'padding-right': isSearchVal ? '20px' : '5px',
        }"
      />
      <place-dropdown
        ref="dropdown"
        v-if="isSearchVal"
        :searchText="contentVal"
        @update:content="updateValue"
        @update:selected="updatePlaceSelected"
        :type="title == '' ? 'text_search' : 'find_place'"
      />
      <font-awesome-icon
        v-if="isSearch"
        class="search-icon"
        icon="fa-solid fa-magnifying-glass"
      />
    </div>
    <p class="error" v-if="errorVal != ''">{{ errorVal }}</p>
  </div>
</template>

<script>
import PlaceDropdown from "./PlaceDropdown.vue";

export default {
  name: "InputField",

  components: {
    PlaceDropdown,
  },

  props: {
    title: String,
    content: String,
    isSearch: Boolean,
    canSearch: Boolean,
    error: String,
  },

  data() {
    return {
      titleVal: this.title,
      contentVal: this.content,
      isSearchVal: this.isSearch,
      firstClick: true,
      prevSearch: "",
      canSearchVal: this.canSearch,
      errorVal: this.error ? this.error : "",
      placeSelected: false,
      selectedPlaces: [],
      midpoint: {},
      radius: 1000,
      startingPosition: {},
      endingPosition: {},
      interval: undefined,
    };
  },

  methods: {
    updateTextColour() {
      const inputField = this.$refs.inputField;
      if (!this.firstClick) {
        inputField.style.color = "var(--grey-med)";
      } else {
        inputField.style.color = "var(--grey-light)";
      }
    },
    searchContent() {
      console.log("?");
      if (this.isSearchVal && this.contentVal != this.prevSearch) {
        if (this.title == "") {
          this.$refs.dropdown.searchPlaces(
            {
              query: this.contentVal,
              midpoint: this.midpoint,
              radius: this.radius,
              startingPosition: this.startingPosition,
              endingPosition: this.endingPosition,
            },
            "text_search"
          );
          this.prevSearch = this.contentVal;
        } else {
          this.$refs.dropdown.searchPlaces(
            { query: this.contentVal },
            "find_place"
          );
          this.prevSearch = this.contentVal;
        }
      }
    },
    updatePlaceSelected() {
      if (this.isSearchVal) {
        this.placeSelected = this.$refs.dropdown.placeSelected;
        this.selectedPlaces = this.$refs.dropdown.selectedPlaces;
        this.$emit("update:selected", this.placeSelected);
      }
    },
    handleContentChange() {
      this.updatePlaceSelected();
      this.updateTextColour();
      this.$emit("update:content", this.contentVal);
    },
    handleContentFocus() {
      if (this.firstClick) {
        this.contentVal = "";
        this.firstClick = false;
      }
      if (this.isSearchVal) {
        this.interval = setInterval(this.searchContent, 1000);
        this.$refs.dropdown.is_open = true;
      }

      this.updateTextColour();
    },
    clear() {
      this.firstClick = true;
      if (this.title == "") {
        this.contentVal = "Search Potential Locations";
      } else {
        this.contentVal = "Search";
      }
      this.handleContentBlur();
      this.$emit("update:content", this.contentVal);
      this.updateTextColour();
    },
    updateValue(event) {
      this.contentVal = this.$refs.dropdown.places[event].name;
      this.updatePlaceSelected();
    },
    handleContentBlur() {
      if (this.$refs.dropdown && this.isSearchVal) {
        this.$refs.dropdown.is_open = false;
      }
      if (this.isSearchVal && this.interval) {
        clearInterval(this.interval);
      }
    },
  },
};
</script>

<style lang="scss">
@import "./InputField.scss";
</style>
