set model1 [load_model -json data/flare.json]

set plot1 [create_plot -model $model1 -type sunburst -columns "names=0,value=1" -title "sunburst"]

#set model2 [load_model -csv data/flare.csv -comment_header]

#set plot2 [create_plot -model $model2 -type sunburst -columns "names=0,value=1" -title "sunburst"]

#set model3 [load_model -tsv data/coffee.tsv -first_line_header]

#set plot3 [create_plot -model $model3 -type sunburst -columns "names=0,color=1" -title "coffee characteristics" -properties "multiRoot=1"]

#set model4 [load_model -csv data/book_revenue.csv -first_line_header]

#set plot4 [create_plot -model $model4 -type sunburst -columns "names=0 1 2,value=3" -title "book revenue"]