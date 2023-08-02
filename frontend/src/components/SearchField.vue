<template>
  <div class="search-field">
    <h2>{{ titleVal }}</h2>
    <div class="search-bar">
      <input
        ref="inputField"
        type="text"
        v-model="contentVal"
        @focus="handleContentFocus"
        @input="handleContentChange"
      />
      <font-awesome-icon
        class="search-icon"
        icon="fa-solid fa-magnifying-glass"
      />
    </div>
  </div>
</template>

<script>
export default {
  name: "SearchField",

  props: {
    title: String,
    content: String,
  },

  data() {
    return {
      titleVal: this.title,
      contentVal: this.content,
      firstClick: true,
    };
  },

  methods: {
    updateTextColour() {
      const inputField = this.$refs.inputField;
      if (inputField) {
        inputField.style.color = "var(--grey-med)";
      }
    },
    handleContentChange() {
      this.updateTextColour();
    },
    handleContentFocus() {
      if (this.firstClick) {
        this.contentVal = "";
        this.firstClick = false;
      }
    },
  },
};
</script>

<style>
.search-field {
  display: flex;
  flex-direction: column;
  align-items: flex-start;
  width: calc(100% - 10px);
  margin-right: 10px;
}

.search-bar {
  display: flex;
  width: 100%;
}

.search-bar input {
  height: 22px;
  width: 100%;
  padding-right: 20px;
  padding-left: 5px;
  flex: 1;
  color: var(--grey-light);
  box-sizing: border-box;
  font-size: 12px;
}

.search-bar .search-icon {
  color: var(--grey-med);
  font-size: 0.75rem;
  transform: translateX(-150%) translateY(55%);
}
</style>
