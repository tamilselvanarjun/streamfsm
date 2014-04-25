#!/usr/bin/python
import os
max_neighbors = range(1,11);
freq_percents = [500]
is_directed = 'u'
data_source = 'ART2'
gspan_freq = 0.1

freq_graph_dir = 'logs'
os.system('mkdir ' + freq_graph_dir)

for max_nbr_count in max_neighbors:
    for f in freq_percents:
        outfile = freq_graph_dir + '/' + 'freqfile_' + data_source \
            + '-' + is_directed + '-MAXNBRS_' + str(max_nbr_count) \
            + '-FREQ_' + str(f) + '-GSPANFREQ_' + str(gspan_freq) + '.graph'

        cmd = './gload ' + str(f) + ' ' + is_directed \
            + ' ' + str(max_nbr_count) + ' ' + data_source + ' ' + str(gspan_freq)
        print('+++++ Running StreamFSM +++++')
        print('Executing: ' + cmd)
        os.system(cmd)
        print('Saving ../cur_exp/freqfile.graph to: ' + outfile)
        os.system('mv ../cur_exp/freqfile.graph ' + outfile)
        os.system('rm ../cur_exp/*')
