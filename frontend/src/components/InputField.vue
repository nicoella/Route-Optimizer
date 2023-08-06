<template>
  <div class="input-field">
    <h2 v-if="titleVal != ''">{{ titleVal }}</h2>
    <div class="input-bar">
      <input
        ref="inputField"
        type="text"
        v-model="contentVal"
        @focus="handleContentFocus"
        @input="handleContentChange"
        :style="{
          'padding-right': isSearchVal ? '20px' : '5px',
        }"
      />
      <place-dropdown
        ref="dropdown"
        v-if="isSearchVal"
        :searchText="contentVal"
      />
      <font-awesome-icon
        v-if="isSearch"
        class="search-icon"
        icon="fa-solid fa-magnifying-glass"
      />
    </div>
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
  },

  data() {
    return {
      titleVal: this.title,
      contentVal: this.content,
      isSearchVal: this.isSearch,
      firstClick: true,
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
    handleContentChange() {
      this.$emit("update:content", this.contentVal);
      this.updateTextColour();
    },
    handleContentFocus() {
      if (this.firstClick) {
        this.contentVal = "";
        this.firstClick = false;
      }
      this.updateTextColour();
      this.$refs.dropdown.searchPlaces(this.contentVal);
    },
    clear() {
      this.firstClick = true;
      this.contentVal = "Search";
      this.$emit("update:content", this.contentVal);
      this.updateTextColour();
    },
    handlePlaceSelection(place) {
      this.contentVal = place.name;
      this.$emit("update:content", this.contentVal);
      this.firstClick = false;
    },
  },
};
</script>

<style lang="scss">
@import "./InputField.scss";
</style>
