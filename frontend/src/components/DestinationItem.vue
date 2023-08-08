<template>
  <div class="destination-item">
    <p>{{ destinationVal }}</p>
    <div class="list">
      <li v-for="option in optionsVal" :key="option.id">
        {{ option.name }}
      </li>
    </div>
  </div>
</template>

<script>
export default {
  name: "DestinationItem",

  props: {
    destination: String,
    options: Array,
  },

  data() {
    return {
      destinationVal: this.destination,
      optionsVal: this.options,
    };
  },

  methods: {
    update(destination, options) {
      this.destinationVal = destination;
      this.optionsVal = options;
    },
  },
};
</script>

<style lang="scss">
@import "./DestinationItem.scss";
</style>
