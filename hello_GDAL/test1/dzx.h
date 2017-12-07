#pragma once

OGRLayer* dzxRes();
//1
OGRLayer * getDZX();
//2 clean
OGRLayer * cleanDZX(OGRLayer* layer);
void getBZC(OGRLayer* layer, double* aue, double* bzc);