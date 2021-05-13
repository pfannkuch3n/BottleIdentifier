#!/bin/python3
import sys
import tensorflow as tf
'''
    Get variable 
    weight = sys.argv[1];
'''
# print ('Argument List:', str(sys.argv[1]))
print (str(sys.argv[0]))

prediction_model = tf.keras.models.load_model('/
