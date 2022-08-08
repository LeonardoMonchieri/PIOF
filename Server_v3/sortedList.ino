
//Print the list of the date
void printTList(List<time_t> l) {
  for (int i = 0; i <l.getSize(); i++) {
    Serial.print("element ");
    Serial.print(i);
    Serial.print(" => ");
    Serial.println(timeToString(l.getValue(i)));
  }
  return;
}


void sortedAddT(time_t element) {
  if (allarms_list.isEmpty()) {
    allarms_list.add(element);
    return;
  }
  for (int i = 0; i < allarms_list.getSize(); i++) {
    if (element >= allarms_list.getValue(i)) {
      allarms_list.addAtIndex(i, element);
      return;
    }
  }
  allarms_list.addLast(element);
  return;
}
