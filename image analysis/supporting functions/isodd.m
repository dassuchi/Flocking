function [allodd,elements]=isodd(x)

%[allodd,elements]=isodd(x)
%
%ISODD.m is a simple utility to determine both whether all elements are odd
%and which elements are odd. 
%
%INCLUDE:
%
%INPUTS:    X:          The elements to be tested. 
%
%OUTPUTS:   ALLODD:     Whether all input elements are odd
%           ELEMENTS:   Which input elements are odd
%
%Created by Stephen Anthony 10/2009 U. Illinois Urbana-Champaign
%Last modified by Stephen Anthony on 10/06/2009


elements=(mod(x+1,2)==0);
allodd=sum(~elements(:))==0;