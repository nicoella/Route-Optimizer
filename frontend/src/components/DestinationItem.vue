<template>
  <div class="destination-item">
    <div class="left">
      <p>{{ destinationVal }}</p>
    </div>
    <div class="list">
      <ul>
        <li v-for="option in optionsVal" :key="option.id">
          <div class="name">
            {{ option.name }}
          </div>
          <div class="address">
            {{ option.address }}
          </div>
        </li>
      </ul>
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
