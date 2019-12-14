# Compile hw3
g++ -std=c++11 -o hw3 hw3.cpp

# Compute linear orders for 100 query graphs
echo -ne 'computing linear orders for 100 query graphs...'
./hw3 yeast yeast_400n 100 > hw3.dag
echo 'done'

# Run daf using the computed DAGs
echo -ne 'running DAF using the computed DAGs...'
./daf_2min -d yeast -q yeast_400n -a hw3.dag -n 100 -m 100000 > result_hw3
echo 'done'

# Run original daf
echo -ne 'running the original DAF...'
./daf_2min -d yeast -q yeast_400n -n 100 -m 100000 > result_daf
echo 'done'

# Compare two results
echo '*comparing result*'
python sort_result.py result_hw3 result_daf
