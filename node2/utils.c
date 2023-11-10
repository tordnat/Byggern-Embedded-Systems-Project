int map(int val_to_map, int in_min, int in_max, int out_min, int out_max) {
  return (val_to_map - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}