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
      if (this.isSearchVal && this.contentVal != this.prevSearch) {
        if (this.title == "") {
          console.log(this.midpoint);
          console.log(this.radius);
          this.$refs.dropdown.searchPlaces(
            {
              query: this.contentVal,
              midpoint: this.midpoint,
              radius: this.radius,
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
      this.placeSelected = this.$refs.dropdown.placeSelected;
      this.selectedPlaces = this.$refs.dropdown.selectedPlaces;
      this.$emit("update:selected", this.placeSelected);
    },
    handleContentChange() {
      this.updatePlaceSelected();
      this.updateTextColour();
      this.$emit("update:content", this.contentVal);
    },
    handleContentFocus() {
      if (this.isSearchVal) {
        this.$refs.dropdown.is_open = true;
      }
      if (this.firstClick) {
        this.contentVal = "";
        this.firstClick = false;
      }
      setInterval(this.searchContent, 1000);
      this.updateTextColour();
    },
    clear() {
      this.firstClick = true;
      this.contentVal = "Search";
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
    },
  },
};
</script>

<style lang="scss">
@import "./InputField.scss";
</style>
