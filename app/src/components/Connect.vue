<template>
   <v-container fluid class="h-100 pa-0">
      <v-row no-gutters justify="center" class="pa-8">
         <p class="text-center">
            Koble til <span class="text-pink-lighten-2 font-weight-black">Partyhatten</span> med
            bluetooth og opplev magien!
         </p>
      </v-row>
      <v-row no-gutters justify="center">
         <v-fade-transition leave-absolute>
            <v-btn
               v-if="!connecting"
               @click="$emit('connect')"
               color="success"
               prepend-icon="mdi-bluetooth"
               >Koble til hatt</v-btn
            >
            <v-progress-circular v-else indeterminate></v-progress-circular>
         </v-fade-transition>
      </v-row>
   </v-container>
</template>

<script setup>
defineProps(["connecting"])
defineEmits(["connect"])
</script>
