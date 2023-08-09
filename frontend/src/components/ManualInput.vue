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
          ref="destinationNameRef"
          :title="''"
          :content="destinationName"
          :isSearch="false"
          :canSearch="true"
          @update:content="updateValue($event, 'destinationName')"
        />
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
export default {
  name: "ManualInput",

  props: {},

  data() {
    return {};
  },

  methods: {},
};
</script>

<style lang="scss">
@import "./ManualInput.scss";
</style>
