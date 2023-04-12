<template>
   <v-container class="pa-0" style="max-width: 400px">
      <v-row no-gutters justify="center" class="px-3 mb-5">
         <p class="text-center">
            I <span class="text-pink-lighten-2 font-weight-black">Beer mode</span> er hatten blitt
            en øl! Under kan du fylle opp ølen!
         </p>
      </v-row>
      <v-row no-gutters justify="center" class="px-3 mb-5 mt-5">
         <v-col>
            <v-slider class="my-1" v-model="fill" max="10" min="0" direction="vertical">
               <template #append>
                  <v-chip style="width: 50px" class="d-flex justify-center">Full</v-chip>
               </template>
               <template #prepend>
                  <v-chip style="width: 50px" class="d-flex justify-center">Tom</v-chip>
               </template>
            </v-slider>
         </v-col>
      </v-row>
   </v-container>
</template>

<script setup>
import { ref, computed, watch } from "vue"

const enc = new TextEncoder()

const emit = defineEmits(["writeValue"])
const fill = ref(7)

let writingValue = false

const fillInt = computed(() => Math.round(fill.value))

watch(fillInt, () => {
   if (writingValue) return

   writingValue = true
   const writeTimeOut = setTimeout(() => {
      console.log("Writing: ", fillInt.value)
      writingValue = false
      emit("writeValue", enc.encode(fillInt.value), "fill")
   }, 200)
})
</script>
