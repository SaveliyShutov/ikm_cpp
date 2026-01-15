file(REMOVE_RECURSE
  "HabitTracker/qml/main.qml"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/habit-tracker_tooling.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
