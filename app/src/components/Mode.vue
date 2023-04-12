<template>
   <v-container fluid class="pa-0 pb-10 mt-10">
      <v-row no-gutters justify="center">
         <v-btn
            :variant="mode === 'matrix' ? 'tonal' : 'plain'"
            class="rounded-e-0"
            @click="$emit('setMode', 'matrix')">
            Egendefinert
         </v-btn>
         <v-divider vertical />
         <v-btn
            :variant="mode === 'motion' ? 'tonal' : 'plain'"
            class="rounded-0"
            @click="$emit('setMode', 'motion')">
            Bevegelse
         </v-btn>
         <v-divider vertical />
         <v-btn
            :variant="mode === 'sound' ? 'tonal' : 'plain'"
            class="rounded-s-0"
            @click="$emit('setMode', 'sound')">
            Lyd
         </v-btn>
      </v-row>
   </v-container>
</template>

<script setup>
defineProps(["mode"])
defineEmits(["setMode"])
</script>
