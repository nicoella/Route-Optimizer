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

  methods: {},
};
</script>

<style>
.destination-item {
  display: flex;
  text-align: left;
}

.destination-item p {
  width: 30%;
  margin-top: 0;
  padding-top: 2px;
}

.destination-item li {
  display: block;
  color: var(--grey-med);
  font-size: 14px;
  font-weight: 400;
  padding: 2px;
}
</style>
