proc objPressed { view plot id } {
  echo "$view $plot $id"

  set inds [get_charts_data -plot $plot -object $id -name inds]

  echo "$inds"
}

set model [load_model -csv data/lincoln-weather.csv -first_line_header]

set_charts_data -model $model -column 0 -name column_type -value "time:format=%Y-%m-%d"

set plot [create_plot -model $model -type distribution -columns "group=CST\[%B\],value=2"]
#set plot [create_plot -model $model -type distribution -columns "group=CST\[%B\],value=Mean Temperature \[F\]"]

connect_chart -plot $plot -from objIdPressed -to objPressed
