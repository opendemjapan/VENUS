#include <stdio.h>
#include "shape.h"
#include <stdlib.h>


int triTable[256][16] = {
// 0 - 49
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 0, 8, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 0, 1, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 1, 8, 3, 9, 8, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 1, 2,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 0, 8, 3, 1, 2,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 9, 2,10, 0, 2, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 2, 8, 3, 2,10, 8,10, 9, 8,-1,-1,-1,-1,-1,-1,-1},
	{ 3,11, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 0,11, 2, 8,11, 0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 1, 9, 0, 2, 3,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 1,11, 2, 1, 9,11, 9, 8,11,-1,-1,-1,-1,-1,-1,-1},
	{ 3,10, 1,11,10, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 0,10, 1, 0, 8,10, 8,11,10,-1,-1,-1,-1,-1,-1,-1},
	{ 3, 9, 0, 3,11, 9,11,10, 9,-1,-1,-1,-1,-1,-1,-1},
	{ 9, 8,10,10, 8,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 4, 7, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 4, 3, 0, 7, 3, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 0, 1, 9, 8, 4, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 4, 1, 9, 4, 7, 1, 7, 3, 1,-1,-1,-1,-1,-1,-1,-1},
	{ 1, 2,10, 8, 4, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 3, 4, 7, 3, 0, 4, 1, 2,10,-1,-1,-1,-1,-1,-1,-1},
	{ 9, 2,10, 9, 0, 2, 8, 4, 7,-1,-1,-1,-1,-1,-1,-1},
	{ 2,10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4,-1,-1,-1,-1},
	{ 8, 4, 7, 3,11, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{11, 4, 7,11, 2, 4, 2, 0, 4,-1,-1,-1,-1,-1,-1,-1},
	{ 9, 0, 1, 8, 4, 7, 2, 3,11,-1,-1,-1,-1,-1,-1,-1},
	{ 4, 7,11, 9, 4,11, 9,11, 2, 9, 2, 1,-1,-1,-1,-1},
	{ 3,10, 1, 3,11,10, 7, 8, 4,-1,-1,-1,-1,-1,-1,-1},
	{ 1,11,10, 1, 4,11, 1, 0, 4, 7,11, 4,-1,-1,-1,-1},
	{ 4, 7, 8, 9, 0,11, 9,11,10,11, 0, 3,-1,-1,-1,-1},
	{ 4, 7,11, 4,11, 9, 9,11,10,-1,-1,-1,-1,-1,-1,-1},
	{ 9, 5, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 9, 5, 4, 0, 8, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 0, 5, 4, 1, 5, 0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 8, 5, 4, 8, 3, 5, 3, 1, 5,-1,-1,-1,-1,-1,-1,-1},
	{ 1, 2,10, 9, 5, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 3, 0, 8, 1, 2,10, 4, 9, 5,-1,-1,-1,-1,-1,-1,-1},
	{ 5, 2,10, 5, 4, 2, 4, 0, 2,-1,-1,-1,-1,-1,-1,-1},
	{ 2,10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8,-1,-1,-1,-1},
	{ 9, 5, 4, 2, 3,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 0,11, 2, 0, 8,11, 4, 9, 5,-1,-1,-1,-1,-1,-1,-1},
	{ 0, 5, 4, 0, 1, 5, 2, 3,11,-1,-1,-1,-1,-1,-1,-1},
	{ 2, 1, 5, 2, 5, 8, 2, 8,11, 4, 8, 5,-1,-1,-1,-1},
	{10, 3,11,10, 1, 3, 9, 5, 4,-1,-1,-1,-1,-1,-1,-1},
	{ 4, 9, 5, 0, 8, 1, 8,10, 1, 8,11,10,-1,-1,-1,-1},
	{ 5, 4, 0, 5, 0,11, 5,11,10,11, 0, 3,-1,-1,-1,-1},
	{ 5, 4, 8, 5, 8,10,10, 8,11,-1,-1,-1,-1,-1,-1,-1},
	{ 9, 7, 8, 5, 7, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 9, 3, 0, 9, 5, 3, 5, 7, 3,-1,-1,-1,-1,-1,-1,-1},
// 50 - 99
	{ 0, 7, 8, 0, 1, 7, 1, 5, 7,-1,-1,-1,-1,-1,-1,-1},
	{ 1, 5, 3, 3, 5, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 9, 7, 8, 9, 5, 7,10, 1, 2,-1,-1,-1,-1,-1,-1,-1},
	{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3,-1,-1,-1,-1},
	{ 8, 0, 2, 8, 2, 5, 8, 5, 7,10, 5, 2,-1,-1,-1,-1},
	{ 2,10, 5, 2, 5, 3, 3, 5, 7,-1,-1,-1,-1,-1,-1,-1},
	{ 7, 9, 5, 7, 8, 9, 3,11, 2,-1,-1,-1,-1,-1,-1,-1},
	{ 9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7,11,-1,-1,-1,-1},
	{ 2, 3,11, 0, 1, 8, 1, 7, 8, 1, 5, 7,-1,-1,-1,-1},
	{11, 2, 1,11, 1, 7, 7, 1, 5,-1,-1,-1,-1,-1,-1,-1},
	{ 9, 5, 8, 8, 5, 7,10, 1, 3,10, 3,11,-1,-1,-1,-1},
	{ 5, 7, 0, 5, 0, 9, 7,11, 0, 1, 0,10,11,10, 0,-1},
	{11,10, 0,11, 0, 3,10, 5, 0, 8, 0, 7, 5, 7, 0,-1},
	{11,10, 5, 7,11, 5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{10, 6, 5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 0, 8, 3, 5,10, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 9, 0, 1, 5,10, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 1, 8, 3, 1, 9, 8, 5,10, 6,-1,-1,-1,-1,-1,-1,-1},
	{ 1, 6, 5, 2, 6, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 1, 6, 5, 1, 2, 6, 3, 0, 8,-1,-1,-1,-1,-1,-1,-1},
	{ 9, 6, 5, 9, 0, 6, 0, 2, 6,-1,-1,-1,-1,-1,-1,-1},
	{ 5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8,-1,-1,-1,-1},
	{ 2, 3,11,10, 6, 5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{11, 0, 8,11, 2, 0,10, 6, 5,-1,-1,-1,-1,-1,-1,-1},
	{ 0, 1, 9, 2, 3,11, 5,10, 6,-1,-1,-1,-1,-1,-1,-1},
	{ 5,10, 6, 1, 9, 2, 9,11, 2, 9, 8,11,-1,-1,-1,-1},
	{ 6, 3,11, 6, 5, 3, 5, 1, 3,-1,-1,-1,-1,-1,-1,-1},
	{ 0, 8,11, 0,11, 5, 0, 5, 1, 5,11, 6,-1,-1,-1,-1},
	{ 3,11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9,-1,-1,-1,-1},
	{ 6, 5, 9, 6, 9,11,11, 9, 8,-1,-1,-1,-1,-1,-1,-1},
	{ 5,10, 6, 4, 7, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 4, 3, 0, 4, 7, 3, 6, 5,10,-1,-1,-1,-1,-1,-1,-1},
	{ 1, 9, 0, 5,10, 6, 8, 4, 7,-1,-1,-1,-1,-1,-1,-1},
	{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4,-1,-1,-1,-1},
	{ 6, 1, 2, 6, 5, 1, 4, 7, 8,-1,-1,-1,-1,-1,-1,-1},
	{ 1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7,-1,-1,-1,-1},
	{ 8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6,-1,-1,-1,-1},
	{ 7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9,-1},
	{ 3,11, 2, 7, 8, 4,10, 6, 5,-1,-1,-1,-1,-1,-1,-1},
	{ 5,10, 6, 4, 7, 2, 4, 2, 0, 2, 7,11,-1,-1,-1,-1},
	{ 0, 1, 9, 4, 7, 8, 2, 3,11, 5,10, 6,-1,-1,-1,-1},
	{ 9, 2, 1, 9,11, 2, 9, 4,11, 7,11, 4, 5,10, 6,-1},
	{ 8, 4, 7, 3,11, 5, 3, 5, 1, 5,11, 6,-1,-1,-1,-1},
	{ 5, 1,11, 5,11, 6, 1, 0,11, 7,11, 4, 0, 4,11,-1},
	{ 0, 5, 9, 0, 6, 5, 0, 3, 6,11, 6, 3, 8, 4, 7,-1},
	{ 6, 5, 9, 6, 9,11, 4, 7, 9, 7,11, 9,-1,-1,-1,-1},
	{10, 4, 9, 6, 4,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 4,10, 6, 4, 9,10, 0, 8, 3,-1,-1,-1,-1,-1,-1,-1},
	{10, 0, 1,10, 6, 0, 6, 4, 0,-1,-1,-1,-1,-1,-1,-1},
	{ 8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1,10,-1,-1,-1,-1},
// 100 - 149
	{ 1, 4, 9, 1, 2, 4, 2, 6, 4,-1,-1,-1,-1,-1,-1,-1},
	{ 3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4,-1,-1,-1,-1},
	{ 0, 2, 4, 4, 2, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{ 8, 3, 2, 8, 2, 4, 4, 2, 6,-1,-1,-1,-1,-1,-1,-1},
    {10, 4, 9,10, 6, 4,11, 2, 3,-1,-1,-1,-1,-1,-1,-1},
    { 0, 8, 2, 2, 8,11, 4, 9,10, 4,10, 6,-1,-1,-1,-1},
    { 3,11, 2, 0, 1, 6, 0, 6, 4, 6, 1,10,-1,-1,-1,-1},
    { 6, 4, 1, 6, 1,10, 4, 8, 1, 2, 1,11, 8,11, 1,-1},
    { 9, 6, 4, 9, 3, 6, 9, 1, 3,11, 6, 3,-1,-1,-1,-1},
    { 8,11, 1, 8, 1, 0,11, 6, 1, 9, 1, 4, 6, 4, 1,-1},
    { 3,11, 6, 3, 6, 0, 0, 6, 4,-1,-1,-1,-1,-1,-1,-1},
    { 6, 4, 8,11, 6, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 7,10, 6, 7, 8,10, 8, 9,10,-1,-1,-1,-1,-1,-1,-1},
    { 0, 7, 3, 0,10, 7, 0, 9,10, 6, 7,10,-1,-1,-1,-1},
    {10, 6, 7, 1,10, 7, 1, 7, 8, 1, 8, 0,-1,-1,-1,-1},
    {10, 6, 7,10, 7, 1, 1, 7, 3,-1,-1,-1,-1,-1,-1,-1},
	{ 1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7,-1,-1,-1,-1},
    { 2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9,-1},
    { 7, 8, 0, 7, 0, 6, 6, 0, 2,-1,-1,-1,-1,-1,-1,-1},
    { 7, 3, 2, 6, 7, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 2, 3,11,10, 6, 8,10, 8, 9, 8, 6, 7,-1,-1,-1,-1},
    { 2, 0, 7, 2, 7,11, 0, 9, 7, 6, 7,10, 9,10, 7,-1},
    { 1, 8, 0, 1, 7, 8, 1,10, 7, 6, 7,10, 2, 3,11,-1},
    {11, 2, 1,11, 1, 7,10, 6, 1, 6, 7, 1,-1,-1,-1,-1},
    { 8, 9, 6, 8, 6, 7, 9, 1, 6,11, 6, 3, 1, 3, 6,-1},
    { 0, 9, 1,11, 6, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 7, 8, 0, 7, 0, 6, 3,11, 0,11, 6, 0,-1,-1,-1,-1},
    { 7,11, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 7, 6,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 3, 0, 8,11, 7, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 0, 1, 9,11, 7, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 8, 1, 9, 8, 3, 1,11, 7, 6,-1,-1,-1,-1,-1,-1,-1},
    {10, 1, 2, 6,11, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 1, 2,10, 3, 0, 8, 6,11, 7,-1,-1,-1,-1,-1,-1,-1},
    { 2, 9, 0, 2,10, 9, 6,11, 7,-1,-1,-1,-1,-1,-1,-1},
    { 6,11, 7, 2,10, 3,10, 8, 3,10, 9, 8,-1,-1,-1,-1},
    { 7, 2, 3, 6, 2, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 7, 0, 8, 7, 6, 0, 6, 2, 0,-1,-1,-1,-1,-1,-1,-1},
    { 2, 7, 6, 2, 3, 7, 0, 1, 9,-1,-1,-1,-1,-1,-1,-1},
    { 1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6,-1,-1,-1,-1},
    {10, 7, 6,10, 1, 7, 1, 3, 7,-1,-1,-1,-1,-1,-1,-1},
    {10, 7, 6, 1, 7,10, 1, 8, 7, 1, 0, 8,-1,-1,-1,-1},
    { 0, 3, 7, 0, 7,10, 0,10, 9, 6,10, 7,-1,-1,-1,-1},
    { 7, 6,10, 7,10, 8, 8,10, 9,-1,-1,-1,-1,-1,-1,-1},
    { 6, 8, 4,11, 8, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 3, 6,11, 3, 0, 6, 0, 4, 6,-1,-1,-1,-1,-1,-1,-1},
    { 8, 6,11, 8, 4, 6, 9, 0, 1,-1,-1,-1,-1,-1,-1,-1},
    { 9, 4, 6, 9, 6, 3, 9, 3, 1,11, 3, 6,-1,-1,-1,-1},
    { 6, 8, 4, 6,11, 8, 2,10, 1,-1,-1,-1,-1,-1,-1,-1},
    { 1, 2,10, 3, 0,11, 0, 6,11, 0, 4, 6,-1,-1,-1,-1},
// 150 - 199
	{ 4,11, 8, 4, 6,11, 0, 2, 9, 2,10, 9,-1,-1,-1,-1},
    {10, 9, 3,10, 3, 2, 9, 4, 3,11, 3, 6, 4, 6, 3,-1},
    { 8, 2, 3, 8, 4, 2, 4, 6, 2,-1,-1,-1,-1,-1,-1,-1},
    { 0, 4, 2, 4, 6, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8,-1,-1,-1,-1},
    { 1, 9, 4, 1, 4, 2, 2, 4, 6,-1,-1,-1,-1,-1,-1,-1},
    { 8, 1, 3, 8, 6, 1, 8, 4, 6, 6,10, 1,-1,-1,-1,-1},
    {10, 1, 0,10, 0, 6, 6, 0, 4,-1,-1,-1,-1,-1,-1,-1},
    { 4, 6, 3, 4, 3, 8, 6,10, 3, 0, 3, 9,10, 9, 3,-1},
    {10, 9, 4, 6,10, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 4, 9, 5, 7, 6,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 0, 8, 3, 4, 9, 5,11, 7, 6,-1,-1,-1,-1,-1,-1,-1},
    { 5, 0, 1, 5, 4, 0, 7, 6,11,-1,-1,-1,-1,-1,-1,-1},
    {11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5,-1,-1,-1,-1},
    { 9, 5, 4,10, 1, 2, 7, 6,11,-1,-1,-1,-1,-1,-1,-1},
    { 6,11, 7, 1, 2,10, 0, 8, 3, 4, 9, 5,-1,-1,-1,-1},
    { 7, 6,11, 5, 4,10, 4, 2,10, 4, 0, 2,-1,-1,-1,-1},
    { 3, 4, 8, 3, 5, 4, 3, 2, 5,10, 5, 2,11, 7, 6,-1},
    { 7, 2, 3, 7, 6, 2, 5, 4, 9,-1,-1,-1,-1,-1,-1,-1},
    { 9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7,-1,-1,-1,-1},
    { 3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0,-1,-1,-1,-1},
    { 6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8,-1},
    { 9, 5, 4,10, 1, 6, 1, 7, 6, 1, 3, 7,-1,-1,-1,-1},
    { 1, 6,10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4,-1},
    { 4, 0,10, 4,10, 5, 0, 3,10, 6,10, 7, 3, 7,10,-1},
    { 7, 6,10, 7,10, 8, 5, 4,10, 4, 8,10,-1,-1,-1,-1},
    { 6, 9, 5, 6,11, 9,11, 8, 9,-1,-1,-1,-1,-1,-1,-1},
    { 3, 6,11, 0, 6, 3, 0, 5, 6, 0, 9, 5,-1,-1,-1,-1},
    { 0,11, 8, 0, 5,11, 0, 1, 5, 5, 6,11,-1,-1,-1,-1},
    { 6,11, 3, 6, 3, 5, 5, 3, 1,-1,-1,-1,-1,-1,-1,-1},
    { 1, 2,10, 9, 5,11, 9,11, 8,11, 5, 6,-1,-1,-1,-1},
    { 0,11, 3, 0, 6,11, 0, 9, 6, 5, 6, 9, 1, 2,10,-1},
	{11, 8, 5,11, 5, 6, 8, 0, 5,10, 5, 2, 0, 2, 5,-1},
    { 6,11, 3, 6, 3, 5, 2,10, 3,10, 5, 3,-1,-1,-1,-1},
    { 5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2,-1,-1,-1,-1},
    { 9, 5, 6, 9, 6, 0, 0, 6, 2,-1,-1,-1,-1,-1,-1,-1},
    { 1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8,-1},
    { 1, 5, 6, 2, 1, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 1, 3, 6, 1, 6,10, 3, 8, 6, 5, 6, 9, 8, 9, 6,-1},
    {10, 1, 0,10, 0, 6, 9, 5, 0, 5, 6, 0,-1,-1,-1,-1},
    { 0, 3, 8, 5, 6,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    {10, 5, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    {11, 5,10, 7, 5,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    {11, 5,10,11, 7, 5, 8, 3, 0,-1,-1,-1,-1,-1,-1,-1},
    { 5,11, 7, 5,10,11, 1, 9, 0,-1,-1,-1,-1,-1,-1,-1},
    {10, 7, 5,10,11, 7, 9, 8, 1, 8, 3, 1,-1,-1,-1,-1},
    {11, 1, 2,11, 7, 1, 7, 5, 1,-1,-1,-1,-1,-1,-1,-1},
    { 0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2,11,-1,-1,-1,-1},
    { 9, 7, 5, 9, 2, 7, 9, 0, 2, 2,11, 7,-1,-1,-1,-1},
    { 7, 5, 2, 7, 2,11, 5, 9, 2, 3, 2, 8, 9, 8, 2,-1},
// 200 - 249
    { 2, 5,10, 2, 3, 5, 3, 7, 5,-1,-1,-1,-1,-1,-1,-1},
    { 8, 2, 0, 8, 5, 2, 8, 7, 5,10, 2, 5,-1,-1,-1,-1},
    { 9, 0, 1, 5,10, 3, 5, 3, 7, 3,10, 2,-1,-1,-1,-1},
    { 9, 8, 2, 9, 2, 1, 8, 7, 2,10, 2, 5, 7, 5, 2,-1},
    { 1, 3, 5, 3, 7, 5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 0, 8, 7, 0, 7, 1, 1, 7, 5,-1,-1,-1,-1,-1,-1,-1},
    { 9, 0, 3, 9, 3, 5, 5, 3, 7,-1,-1,-1,-1,-1,-1,-1},
    { 9, 8, 7, 5, 9, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 5, 8, 4, 5,10, 8,10,11, 8,-1,-1,-1,-1,-1,-1,-1},
    { 5, 0, 4, 5,11, 0, 5,10,11,11, 3, 0,-1,-1,-1,-1},
    { 0, 1, 9, 8, 4,10, 8,10,11,10, 4, 5,-1,-1,-1,-1},
    {10,11, 4,10, 4, 5,11, 3, 4, 9, 4, 1, 3, 1, 4,-1},
    { 2, 5, 1, 2, 8, 5, 2,11, 8, 4, 5, 8,-1,-1,-1,-1},
    { 0, 4,11, 0,11, 3, 4, 5,11, 2,11, 1, 5, 1,11,-1},
    { 0, 2, 5, 0, 5, 9, 2,11, 5, 4, 5, 8,11, 8, 5,-1},
    { 9, 4, 5, 2,11, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 2, 5,10, 3, 5, 2, 3, 4, 5, 3, 8, 4,-1,-1,-1,-1},
    { 5,10, 2, 5, 2, 4, 4, 2, 0,-1,-1,-1,-1,-1,-1,-1},
    { 3,10, 2, 3, 5,10, 3, 8, 5, 4, 5, 8, 0, 1, 9,-1},
    { 5,10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2,-1,-1,-1,-1},
    { 8, 4, 5, 8, 5, 3, 3, 5, 1,-1,-1,-1,-1,-1,-1,-1},
    { 0, 4, 5, 1, 0, 5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5,-1,-1,-1,-1},
    { 9, 4, 5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 4,11, 7, 4, 9,11, 9,10,11,-1,-1,-1,-1,-1,-1,-1},
    { 0, 8, 3, 4, 9, 7, 9,11, 7, 9,10,11,-1,-1,-1,-1},
    { 1,10,11, 1,11, 4, 1, 4, 0, 7, 4,11,-1,-1,-1,-1},
    { 3, 1, 4, 3, 4, 8, 1,10, 4, 7, 4,11,10,11, 4,-1},
    { 4,11, 7, 9,11, 4, 9, 2,11, 9, 1, 2,-1,-1,-1,-1},
    { 9, 7, 4, 9,11, 7, 9, 1,11, 2,11, 1, 0, 8, 3,-1},
    {11, 7, 4,11, 4, 2, 2, 4, 0,-1,-1,-1,-1,-1,-1,-1},
    {11, 7, 4,11, 4, 2, 8, 3, 4, 3, 2, 4,-1,-1,-1,-1},
    { 2, 9,10, 2, 7, 9, 2, 3, 7, 7, 4, 9,-1,-1,-1,-1},
    { 9,10, 7, 9, 7, 4,10, 2, 7, 8, 7, 0, 2, 0, 7,-1},
    { 3, 7,10, 3,10, 2, 7, 4,10, 1,10, 0, 4, 0,10,-1},
    { 1,10, 2, 8, 7, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 4, 9, 1, 4, 1, 7, 7, 1, 3,-1,-1,-1,-1,-1,-1,-1},
    { 4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1,-1,-1,-1,-1},
    { 4, 0, 3, 7, 4, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 4, 8, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 9,10, 8,10,11, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 3, 0, 9, 3, 9,11,11, 9,10,-1,-1,-1,-1,-1,-1,-1},
    { 0, 1,10, 0,10, 8, 8,10,11,-1,-1,-1,-1,-1,-1,-1},
    { 3, 1,10,11, 3,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 1, 2,11, 1,11, 9, 9,11, 8,-1,-1,-1,-1,-1,-1,-1},
    { 3, 0, 9, 3, 9,11, 1, 2, 9, 2,11, 9,-1,-1,-1,-1},
    { 0, 2,11, 8, 0,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 3, 2,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 2, 3, 8, 2, 8,10,10, 8, 9,-1,-1,-1,-1,-1,-1,-1},
    { 9,10, 2, 0, 9, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
// 250 - 255
	{ 2, 3, 8, 2, 8,10, 0, 1, 8, 1,10, 8,-1,-1,-1,-1},
    { 1,10, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 1, 3, 8, 9, 1, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 0, 9, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    { 0, 3, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
    };

int edgeTable[] = {
      0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
      0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
      0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
      0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
      0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
      0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
      0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
      0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
      0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
      0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
      0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
      0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
      0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
      0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
      0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
      0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
      0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
      0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
      0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
      0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
      0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
      0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
      0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
      0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
      0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
      0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
      0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
      0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
      0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
      0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
      0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
      0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0};  

int triTable_plane[16][8] = {
	{ 0, 1, 2, 3,-1,-1, 4, 1},
	{ 1, 2, 3, 7, 4,-1, 5, 1},
	{ 0, 4, 5, 2, 3,-1, 5, 1},
	{ 2, 3, 7, 5,-1,-1, 4, 1},
	{ 0, 1, 5, 6, 3,-1, 5, 1},
	{ 1, 5, 4, 3, 7, 6, 3, 2},
	{ 0, 4, 6, 3,-1,-1, 4, 1},
	{ 3, 7, 6,-1,-1,-1, 3, 1},
	{ 0, 1, 2, 6, 7,-1, 5, 1},
	{ 1, 2, 6, 4,-1,-1, 4, 1},
	{ 0, 4, 7, 2, 6, 5, 3, 2},
	{ 2, 6, 5,-1,-1,-1, 3, 1},
	{ 0, 1, 5, 7,-1,-1, 4, 1},
	{ 1, 5, 4,-1,-1,-1, 3, 1},
	{ 0, 4, 7,-1,-1,-1, 3, 1},
	{-1,-1,-1,-1,-1,-1,-1,-1}};

TRIANGLE triangles[16]; 
SLICES poly[2];
int cPNum;
float colorindex[3][3];

int ntriangles;
int ivert;
int total_triangles = 0;
int final_triangles = 0;
int total_polygons;
int nVpolygon;
float f2o[3][3];

int fID;
int sl_type_ID;

/**************************************************************************************************/
void convert_to_ortho(float cell[6])
{
	int i,j;
	float cos_angles[3],sin_angles[3];
	float volume, cos_a1, sin_a1;

// Zero matrices
	for(i=0; i<3; i++){
		for(j=0; j<3; j++) f2o[i][j] = 0;
	}
// Set up sines and cosines
	for(i=0; i<3; i++){
		cos_angles[i] = (float)cos(PI*cell[i+3]/180);
		sin_angles[i] = (float)sin(PI*cell[i+3]/180);
	}
// Compute cell volume
	volume = (float) (cell[0] * cell[1] * cell[2] * 
		     sqrt(1 + 2 * cos_angles[0] * cos_angles[1] * cos_angles[2] - cos_angles[0]*cos_angles[0] 
			 - cos_angles[1]*cos_angles[1] - cos_angles[2]*cos_angles[2]));
// Compute some terms
    cos_a1 =(float) (cos_angles[1] * cos_angles[2] - cos_angles[0]) / (sin_angles[1] * sin_angles[2]);
	sin_a1 = (float) sqrt(1 - cos_a1*cos_a1);

	f2o[0][0] =   cell[0];
	f2o[0][1] =   cos_angles[2] * cell[1];
	f2o[0][2] =   cos_angles[1] * cell[2];

	f2o[1][0] =   0;
	f2o[1][1] =   sin_angles[2] * cell[1];
	f2o[1][2] = - sin_angles[1] * cos_a1 * cell[2];

	f2o[2][0] =   0;
	f2o[2][1] =   0;
	f2o[2][2] =   sin_angles[1] * sin_a1 * cell[2];
}


/**************************************************************************************************/
XYZ VertexInterp(isolevel,p1,p2,valp1,valp2)
float isolevel;
XYZ p1,p2;
float valp1,valp2;
{
	float mu;
	XYZ p;

	if (fabs(isolevel-valp1) < 0.00001) {
		return(p1);
	}
	if (fabs(isolevel-valp2) < 0.00001) {
		return(p2);
	}
	if (fabs(valp1-valp2) < 0.00001) {
		return(p1);
	}
	mu = (float)((isolevel - valp1) / (valp2 - valp1));
	p.x = p1.x + mu * (p2.x - p1.x);
	p.y = p1.y + mu * (p2.y - p1.y);
	p.z = p1.z + mu * (p2.z - p1.z);

	return(p);
}

/**************************************************************************************************/
int polygonize(GRIDCELL grid, float isolevel, TRIANGLE triangles[12])
{
    int i, ntriang;
    int cubeindex;
	XYZ vertlist[12];
	float x,y,z;

// Determine the index into the edge table which tells us 
// which vertices are inside of the surface
	cubeindex = 0;
	if (grid.val[0] < isolevel) cubeindex |= 1;
	if (grid.val[1] < isolevel) cubeindex |= 2;
	if (grid.val[2] < isolevel) cubeindex |= 4;
	if (grid.val[3] < isolevel) cubeindex |= 8;
	if (grid.val[4] < isolevel) cubeindex |= 16;
	if (grid.val[5] < isolevel) cubeindex |= 32;
	if (grid.val[6] < isolevel) cubeindex |= 64;
	if (grid.val[7] < isolevel) cubeindex |= 128;

// Cube is entirely in/out of the surface 
	if (edgeTable[cubeindex] == 0) return(0);

// Find the vertices where the surface intersects the cube 
	if (edgeTable[cubeindex] & 1)    vertlist[ 0] = VertexInterp(isolevel,grid.p[0],grid.p[1],grid.val[0],grid.val[1]);
	if (edgeTable[cubeindex] & 2)    vertlist[ 1] = VertexInterp(isolevel,grid.p[1],grid.p[2],grid.val[1],grid.val[2]);
	if (edgeTable[cubeindex] & 4)    vertlist[ 2] = VertexInterp(isolevel,grid.p[2],grid.p[3],grid.val[2],grid.val[3]);
	if (edgeTable[cubeindex] & 8)    vertlist[ 3] = VertexInterp(isolevel,grid.p[3],grid.p[0],grid.val[3],grid.val[0]);
	if (edgeTable[cubeindex] & 16)   vertlist[ 4] = VertexInterp(isolevel,grid.p[4],grid.p[5],grid.val[4],grid.val[5]);
	if (edgeTable[cubeindex] & 32)   vertlist[ 5] = VertexInterp(isolevel,grid.p[5],grid.p[6],grid.val[5],grid.val[6]);
	if (edgeTable[cubeindex] & 64)   vertlist[ 6] = VertexInterp(isolevel,grid.p[6],grid.p[7],grid.val[6],grid.val[7]);
	if (edgeTable[cubeindex] & 128)  vertlist[ 7] = VertexInterp(isolevel,grid.p[7],grid.p[4],grid.val[7],grid.val[4]);
	if (edgeTable[cubeindex] & 256)  vertlist[ 8] = VertexInterp(isolevel,grid.p[0],grid.p[4],grid.val[0],grid.val[4]);
	if (edgeTable[cubeindex] & 512)  vertlist[ 9] = VertexInterp(isolevel,grid.p[1],grid.p[5],grid.val[1],grid.val[5]);
	if (edgeTable[cubeindex] & 1024) vertlist[10] = VertexInterp(isolevel,grid.p[2],grid.p[6],grid.val[2],grid.val[6]);
	if (edgeTable[cubeindex] & 2048) vertlist[11] = VertexInterp(isolevel,grid.p[3],grid.p[7],grid.val[3],grid.val[7]);

// Create the triangle 
	ntriang = 0;
	for(i=0; triTable[cubeindex][i]!=-1; i+=3){
		x = vertlist[triTable[cubeindex][i  ]].x;
		y = vertlist[triTable[cubeindex][i  ]].y;
		z = vertlist[triTable[cubeindex][i  ]].z;
		triangles[ntriang].p[0].x = x;
		triangles[ntriang].p[0].y = y;
		triangles[ntriang].p[0].z = z;

		x = vertlist[triTable[cubeindex][i+1]].x;
		y = vertlist[triTable[cubeindex][i+1]].y;
		z = vertlist[triTable[cubeindex][i+1]].z;
		triangles[ntriang].p[1].x = x;
		triangles[ntriang].p[1].y = y;
		triangles[ntriang].p[1].z = z;

		x = vertlist[triTable[cubeindex][i+2]].x;
		y = vertlist[triTable[cubeindex][i+2]].y;
		z = vertlist[triTable[cubeindex][i+2]].z;
		triangles[ntriang].p[2].x = x;
		triangles[ntriang].p[2].y = y;
		triangles[ntriang].p[2].z = z;
		ntriang++;
	}
	return(ntriang);
}


/**************************************************************************************************/
int render(float rho[MAX_PIX][MAX_PIX][MAX_PIX], int mygrid[3], int ib[3], int ll[3], int ioxyz[3],
		    float conlevel, float f2o[3][3])
{
	int i,j,k,m,km,jm,ii,N;
	int I1,I2,I3;
	int II1,II2,II3;
//	int ivert;
	int r[3][8]; 
	GRIDCELL rt;
	float v[3][3], vx[3][3];
	float d1[3],d2[3],d3[3],n[3];
	float rl1,rl2,rl3;
	float area;
	int signID;

	r[0][0] = 0; r[1][0] = 0; r[2][0] = 0;
	r[0][1] = 1; r[1][1] = 0; r[2][1] = 0;
	r[0][2] = 1; r[1][2] = 1; r[2][2] = 0;
	r[0][3] = 0; r[1][3] = 1; r[2][3] = 0;
	r[0][4] = 0; r[1][4] = 0; r[2][4] = 1;
	r[0][5] = 1; r[1][5] = 0; r[2][5] = 1;
	r[0][6] = 1; r[1][6] = 1; r[2][6] = 1;
	r[0][7] = 0; r[1][7] = 1; r[2][7] = 1;

	ivert = 0;
    total_triangles = 0;
	final_triangles = 0;

// Loop boxes
	N = 1;
    if (fID != 0) N = 2;
//    if (fID == 1) N = 2;
	for (k=ll[2]; k<(ll[2] + ib[2] - 1); k++){
		for (j=ll[1]; j<(ll[1] + ib[1] - 1); j++){
			for (i=ll[0]; i<(ll[0] + ib[0] - 1); i++){
                for(ii=0; ii<N; ii++){
				signID = 0;
				for (m=0; m<8; m++){
					I1 = i + r[0][m] + ioxyz[0];
					I2 = j + r[1][m] + ioxyz[1];
					I3 = k + r[2][m] + ioxyz[2];
					II1 = I1;
					II2 = I2;
					II3 = I3;
					if (I1 >= mygrid[0]) I1 = I1 - mygrid[0]+1;
					if (I2 >= mygrid[1]) I2 = I2 - mygrid[1]+1;
					if (I3 >= mygrid[2]) I3 = I3 - mygrid[2]+1;
					rt.p[m].x = (float) II1; 
					rt.p[m].y = (float) II2; 
					rt.p[m].z = (float) II3; 
					switch(fID){
					case(0):{
						if (rho[I1][I2][I3] <= 0.0) {
							rt.val[m] = -rho[I1][I2][I3];
							signID++;
						}
						else rt.val[m] = rho[I1][I2][I3];
						break;
							}
					case(1):{
						rt.val[m] = rho[I1][I2][I3];
						if (ii == 1) {
							if (rho[I1][I2][I3] > 0.0) {
								rt.val[m] = -1.0f;
							}
							else rt.val[m] = -rho[I1][I2][I3];
							signID = -1;
						}
						else{
							if (rho[I1][I2][I3] < 0.0) rt.val[m] = -1.0f;
							signID = 0;
						}
						break;
							}
					}
///					if (rho[I1][I2][I3] <= 0.0) {
///						rt.val[m] = (float)(sgn*rho[I1][I2][I3]);
///						signID++;
///					}
///					else rt.val[m] = rho[I1][I2][I3];
				} //--- m loop
				if (fID == 0){
					if (signID > 4) signID = -1;
					else signID = 0;
				}

				ntriangles = polygonize(rt, conlevel, triangles);         // Compute triangles
				if (ntriangles > 0) {
					for (m=0; m<ntriangles; m++){      // Loop all triangles
						for (km=0; km<3; km++){        // Loop all vertices
							v[0][km] = (float)triangles[m].p[km].x - 1;
							v[1][km] = (float)triangles[m].p[km].y - 1;
							v[2][km] = (float)triangles[m].p[km].z - 1;
							for (jm=0; jm<3; jm++){    // Loop all vertex coordinates
								v[jm][km] = (float)(v[jm][km] / mygrid[jm]);  // Convert to grid fractional
							} //--- jm loop
							for (jm=0; jm<3; jm++){    // Convert to orthogonal angstroms
								vx[jm][km] = f2o[jm][0]*v[0][km] + 
                                             f2o[jm][1]*v[1][km] + 
                                             f2o[jm][2]*v[2][km];
								ISO[jm][ivert] = vx[jm][km];
							}
							if (signID == -1) sign[ivert] = '-';
							else sign[ivert] = '+';
							ivert = ivert + 1;
							if (ivert > MAXVERT){
								printf("==================================================\n");
								printf("Number of triangle vertices out of range\n");
								printf("Increase initial isosurface level.\n");
								printf("==================================================\n");
								return -1;
							}
						} //--- km loop
// Compute normal
						d1[0] = vx[0][0] - vx[0][1];
						d1[1] = vx[1][0] - vx[1][1];
						d1[2] = vx[2][0] - vx[2][1];
						d2[0] = vx[0][1] - vx[0][2];
						d2[1] = vx[1][1] - vx[1][2];
						d2[2] = vx[2][1] - vx[2][2];
						d3[0] = vx[0][0] - vx[0][2];
						d3[1] = vx[1][0] - vx[1][2];
						d3[2] = vx[2][0] - vx[2][2];
						rl1 = (float)sqrt(d1[0]*d1[0] + d1[1]*d1[1] + d1[2]*d1[2]);
						rl2 = (float)sqrt(d2[0]*d2[0] + d2[1]*d2[1] + d2[2]*d2[2]);
						rl3 = (float)sqrt(d3[0]*d3[0] + d3[1]*d3[1] + d3[2]*d3[2]);
// If side vectors too short
						if ((rl1 <= 0.0001) || (rl2 <= 0.0001) || (rl3 <= 0.0001)) {ivert = ivert - 3;}
						else {
							n[0] = d1[1]*d2[2] - d2[1]*d1[2];
							n[1] = d1[2]*d2[0] - d2[2]*d1[0];
							n[2] = d1[0]*d2[1] - d2[0]*d1[1];
							area = n[0]*n[0] + n[1]*n[1] + n[2]*n[2];
							n[0] = n[0] / (float)sqrt(area);
							n[1] = n[1] / (float)sqrt(area);
							n[2] = n[2] / (float)sqrt(area);
							n[0] = n[0] / area;
							n[1] = n[1] / area;
							n[2] = n[2] / area;
						}
// Store for preceding three vertices
						for(jm=0; jm<3; jm++){
							ISO[jm+3][ivert-1] = n[jm];
							ISO[jm+3][ivert-2] = n[jm];
							ISO[jm+3][ivert-3] = n[jm];
						}
					} //--- m loop
					total_triangles = total_triangles + ntriangles;
				} //--- if (ntriangles > 0)

				}  //--- for(ii=0; ii<fID; ii++)



			} //--- i loop
		} //--- j loop
	} //--- k loop
//................................................................................................
    return 0;
}


//=================================================================================//
//                   Calculate Triangles for Izosurfase                            //
//=================================================================================//
int shape(float cell[6], float rho[MAX_PIX][MAX_PIX][MAX_PIX],float origin[3], float conlevel, int maxNP[3],
	      int PX0, int PY0, int PZ0, int NPX, int NPY, int NPZ,int ID)
{
	int i;
	int box_point[3],box[3], box_min[3], box_shift[3];

	fID = ID;
	convert_to_ortho(cell);

	box_point[0] = maxNP[0]+1; 
	box_point[1] = maxNP[1]+1; 
	box_point[2] = maxNP[2]+1;
	box[0] = (NPX + 1) - PX0; 
	box[1] = (NPY + 1) - PY0; 
	box[2] = (NPZ + 1) - PZ0;
	box_min[0] = PX0; 
	box_min[1] = PY0; 
	box_min[2] = PZ0;

	for (i=0; i<3; i++) {
		box_shift[i] = (int)(box_point[i]*origin[i]);
	}
	if (render(rho, box_point, box, box_min, box_shift,conlevel, f2o) != -1) {
		final_triangles = ivert;
		printf("Total number of triangles (isosurface) generated: %i\n",total_triangles);
		return(final_triangles);
	}
	return -1;
}



/**************************************************************************************************/
void set_Ortho_Cell(int PX0, int PY0, int PZ0, int NPX, int NPY, int NPZ)
{
	int i,j;
	float r[3];

	uCello[0][0] = (float)PX0; uCello[0][1] = (float)PY0; uCello[0][2] = (float)PZ0;
	uCello[1][0] = (float)NPX; uCello[1][1] = (float)PY0; uCello[1][2] = (float)PZ0;
	uCello[2][0] = (float)NPX; uCello[2][1] = (float)PY0; uCello[2][2] = (float)NPZ;
	uCello[3][0] = (float)PX0; uCello[3][1] = (float)PY0; uCello[3][2] = (float)NPZ;
	uCello[4][0] = (float)PX0; uCello[4][1] = (float)NPY; uCello[4][2] = (float)NPZ;
	uCello[5][0] = (float)NPX; uCello[5][1] = (float)NPY; uCello[5][2] = (float)NPZ;
	uCello[6][0] = (float)NPX; uCello[6][1] = (float)NPY; uCello[6][2] = (float)PZ0;
	uCello[7][0] = (float)PX0; uCello[7][1] = (float)NPY; uCello[7][2] = (float)PZ0;

	for(i=0; i<8; i++) {
		uCello[i][0] = (uCello[i][0] - 1)/(float)(NPX + 1);
		uCello[i][1] = (uCello[i][1] - 1)/(float)(NPY + 1);
		uCello[i][2] = (uCello[i][2] - 1)/(float)(NPZ + 1);
		for (j=0; j<3; j++){
			r[j] = f2o[j][0]*uCello[i][0] + f2o[j][1]*uCello[i][1] + f2o[j][2]*uCello[i][2];
		}
		uCello[i][0] = (float)r[0];
		uCello[i][1] = (float)r[1];
		uCello[i][2] = (float)r[2];
	}
}

/**************************************************************************************************/
void set_Ortho_Translations(int NPX, int NPY, int NPZ)
{
	int i,j;
	float Trans[3][3];
	float r[3];

	Trans[0][0] = ((float)NPX)/((float)NPX + 1);
	Trans[0][1] = 0;
	Trans[0][2] = 0;

	Trans[1][0] = 0;
	Trans[1][1] = ((float)NPY)/((float)NPY + 1);
	Trans[1][2] = 0;
	
	Trans[2][0] = 0;
	Trans[2][1] = 0;
	Trans[2][2] = ((float)NPZ)/((float)NPZ + 1);

	for(i=0; i<3; i++) {
		for(j=0; j<3; j++){
			r[j] = f2o[j][0]*Trans[i][0] + f2o[j][1]*Trans[i][1] + f2o[j][2]*Trans[i][2];
		}
		uTranso[i][0] = (float)r[0];
		uTranso[i][1] = (float)r[1];
		uTranso[i][2] = (float)r[2];
	}
}


/**************************************************************************************************/
int polygonize_plane(GRIDCELL grid, float isolevel,float seclevel,float Dmin,float Dmax)
{
	int i,j,ntriang,I1;
    int squareindex;
	XYZ vertlist[8];
	float x,y,z;
	float den,densp[8];

// Determine the index into the edge table which tells us 
// which vertices are inside of the plane surface
	nVpolygon = 0;
	squareindex = 0;
	ntriang = 0;

//	for (i=0; i<8; i++) densp[i] = isolevel - Dmin;
//	for (i=0; i<4; i++) densp[i] = grid.val[i] - Dmin;
	for (i=0; i<8; i++) densp[i] = isolevel;
	for (i=0; i<4; i++) densp[i] = grid.val[i];
	if (grid.val[0] < isolevel) squareindex |= 1;
	if (grid.val[1] < isolevel) squareindex |= 2;
	if (grid.val[2] < isolevel) squareindex |= 4;
	if (grid.val[3] < isolevel) squareindex |= 8;

// Cube is entirely in/out of the surface 
	if (squareindex == 15) return(0);

	for(i=0; i<4; i++) {
		vertlist[i].x = grid.p[i].x;
		vertlist[i].y = grid.p[i].y;
		vertlist[i].z = grid.p[i].z;
	}
	vertlist[4] = VertexInterp(isolevel,grid.p[0],grid.p[1],grid.val[0],grid.val[1]);
	vertlist[5] = VertexInterp(isolevel,grid.p[1],grid.p[2],grid.val[1],grid.val[2]);
	vertlist[6] = VertexInterp(isolevel,grid.p[2],grid.p[3],grid.val[2],grid.val[3]);
	vertlist[7] = VertexInterp(isolevel,grid.p[3],grid.p[0],grid.val[3],grid.val[0]);

// Create the triangle 
	ntriang = triTable_plane[squareindex][7];
	nVpolygon = triTable_plane[squareindex][6];

	for(i=0; i<ntriang; i++){
		for(j=0; j<nVpolygon; j++){
			x = vertlist[triTable_plane[squareindex][j+i]].x;
			y = vertlist[triTable_plane[squareindex][j+i]].y;
			z = vertlist[triTable_plane[squareindex][j+i]].z;
			poly[i].p[j].x = x;
			poly[i].p[j].y = y;
			poly[i].p[j].z = z;

			I1 = triTable_plane[squareindex][j+i];
			den = densp[I1] / (seclevel*(Dmax - Dmin));
			if (den > 1) den = 1;
			poly[i].Index[j] = (int)(den*99);
		}
	}
	return(ntriang);
}


/**************************************************************************************************/
int section(int ID, float rho[MAX_PIX][MAX_PIX][MAX_PIX], float origin[3], float conlevel, float intlevel, int maxNP[3],
		     int PX0, int PY0, int PZ0, int NPX, int NPY, int NPZ,float Dmin, float Dmax, int type_ID)
{
	int box_point[3],box[3], box_min[3], box_shift[3];
	int r[3][4]; 
	GRIDCELL rt;
	int from[2],to[2];
	int square_plane;
	int I1,I2,I3;
	int II1,II2,II3;
	int i,j,m,km,jm;
	float v[3][5],vx[3][5];

	sl_type_ID = type_ID;

	box_point[0] = maxNP[0]+1; 
	box_point[1] = maxNP[1]+1; 
	box_point[2] = maxNP[2]+1;
	box[0] = (NPX + 1) - PX0; 
	box[1] = (NPY + 1) - PY0; 
	box[2] = (NPZ + 1) - PZ0;
	box_min[0] = PX0; 
	box_min[1] = PY0; 
	box_min[2] = PZ0;

	for (i=0; i<3; i++) {
		box_shift[i] = (int)(box_point[i]*origin[i]);
	}

//-------------------------------------------------------
//  ID = 0  ---  (100) plane;  origin PX0
//  ID = 1  ---  (010) plane;  origin PY0
//  ID = 2  ---  (001) plane;  origin PZ0
//  ID = 3  ---  (100) plane;  origin NPX
//  ID = 4  ---  (010) plane;  origin NPY
//  ID = 5  ---  (001) plane;  origin NPZ
//-------------------------------------------------------
	switch(ID)
	{
//------------------------------------------------------- (100)
	case(3):
	case(0):{
		r[0][0] = 0; r[1][0] = 0; r[2][0] = 0;
		r[0][1] = 0; r[1][1] = 1; r[2][1] = 0;
		r[0][2] = 0; r[1][2] = 1; r[2][2] = 1;
		r[0][3] = 0; r[1][3] = 0; r[2][3] = 1;
		if (ID == 0) I1 = PX0 + box_shift[0];
		else I1 = NPX + box_shift[0];
		II1 = I1;
		if (I1 >= box_point[0]) I1 = I1 - box_point[0]+1;

		from[0] = box_min[2];
		  to[0] = box_min[2] + box[2] - 1;
		from[1] = box_min[1];
		  to[1] = box_min[1] + box[1] - 1;
		break;
			}
//------------------------------------------------------- (010)
	case(4):
	case(1):{
		r[0][0] = 0; r[1][0] = 0; r[2][0] = 0;
		r[0][1] = 1; r[1][1] = 0; r[2][1] = 0;
		r[0][2] = 1; r[1][2] = 0; r[2][2] = 1;
		r[0][3] = 0; r[1][3] = 0; r[2][3] = 1;
		if (ID == 1) I2 = PY0 + box_shift[1];
		else I2 = NPY + box_shift[1];
		II2 = I2;
		if (I2 >= box_point[1]) I2 = I2 - box_point[1]+1;
		from[0] = box_min[2];
		  to[0] = box_min[2] + box[2] - 1;
		from[1] = box_min[0];
		  to[1] = box_min[0] + box[0] - 1;
		break;
			}
//------------------------------------------------------- (001)
	case(5):
	case(2):{
		r[0][0] = 0; r[1][0] = 0; r[2][0] = 0;
		r[0][1] = 1; r[1][1] = 0; r[2][1] = 0;
		r[0][2] = 1; r[1][2] = 1; r[2][2] = 0;
		r[0][3] = 0; r[1][3] = 1; r[2][3] = 0;
		if (ID == 2) I3 = PZ0 + box_shift[2];
		else I3 = NPZ + box_shift[2];
		II3 = I3;
		if (I3 >= box_point[2]) I3 = I3 - box_point[2]+1;
		from[0] = box_min[1];
		  to[0] = box_min[1] + box[1] - 1;
		from[1] = box_min[0];
		  to[1] = box_min[0] + box[0] - 1;
		break;
			}
	}
	total_polygons = 0;
	for (j=from[0]; j<to[0]; j++){
		for (i=from[1]; i<to[1]; i++){
			for (m=0; m<4; m++){
				switch(ID)
				{
				case(3):
				case(0):{
					I2 = i + r[1][m] + box_shift[1];
					I3 = j + r[2][m] + box_shift[2];
					II2 = I2;
					II3 = I3;
					if (I2 >= box_point[1]) I2 = I2 - box_point[1]+1;
					if (I3 >= box_point[2]) I3 = I3 - box_point[2]+1;
					break;
						}

				case(4):
				case(1):{
					I1 = i + r[0][m] + box_shift[0];
					I3 = j + r[2][m] + box_shift[2];
					II1 = I1;
					II3 = I3;
					if (I1 >= box_point[0]) I1 = I1 - box_point[0]+1;
					if (I3 >= box_point[2]) I3 = I3 - box_point[2]+1;
					break;
						}

				case(5):
				case(2):{
					I1 = i + r[0][m] + box_shift[0];
					I2 = j + r[1][m] + box_shift[1];
					II1 = I1;
					II2 = I2;
					if (I1 >= box_point[0]) I1 = I1 - box_point[0]+1;
					if (I2 >= box_point[1]) I2 = I2 - box_point[1]+1;
					break;
						}
				}
				rt.p[m].x = (float) II1; 
				rt.p[m].y = (float) II2; 
				rt.p[m].z = (float) II3; 
				switch(sl_type_ID){
				case(0):{
					if (rho[I1][I2][I3] < 0) rt.val[m] = -rho[I1][I2][I3];
					else rt.val[m] = rho[I1][I2][I3];
					break;
						}
				case(1):{
					if (rho[I1][I2][I3] < 0) rt.val[m] = 0;
					else rt.val[m] = rho[I1][I2][I3];
					break;
						}
				case(2):{
					if (rho[I1][I2][I3] > 0) rt.val[m] = 0;
					else rt.val[m] = -rho[I1][I2][I3];
					break;
						}
				}
			}
			square_plane = polygonize_plane(rt,conlevel,intlevel,Dmin,Dmax);
			if (square_plane > 0){
				for (m=0; m<square_plane; m++){                        // Loop all polygons
					slice[ID][total_polygons].nV = nVpolygon;
					for (km=0; km<nVpolygon; km++){                    // Loop all vertices
						slice[ID][total_polygons].sl.Index[km] = poly[m].Index[km];
						v[0][km] = (float)poly[m].p[km].x - 1;
						v[1][km] = (float)poly[m].p[km].y - 1;
						v[2][km] = (float)poly[m].p[km].z - 1;
						v[0][km] = (float)(v[0][km] / box_point[0]);   // Convert to grid fractional
						v[1][km] = (float)(v[1][km] / box_point[1]);   // Convert to grid fractional
						v[2][km] = (float)(v[2][km] / box_point[2]);   // Convert to grid fractional
						for (jm=0; jm<3; jm++){                        // Convert to orthogonal angstroms
							vx[jm][km] = f2o[jm][0]*v[0][km] + 
                                         f2o[jm][1]*v[1][km] + 
                                         f2o[jm][2]*v[2][km];
						} //--- jm loop
						slice[ID][total_polygons].sl.p[km].x = vx[0][km];
						slice[ID][total_polygons].sl.p[km].y = vx[1][km];
						slice[ID][total_polygons].sl.p[km].z = vx[2][km];
					} //--- km loop
					total_polygons++;
					if (total_polygons > MAXPOLYGON){
						printf("==================================================\n");
						printf("Number of polygons out of range\n");
						printf("Increase initial isosurface level.\n");
						printf("==================================================\n");
						return -1;
					}
				} //--- m loop
			} //--- if (square_plane > 0)
		} //--- i loop
	} //--- j loop
	printf("Total number of polygons (slice) generated: %i\n",total_polygons);

	return(total_polygons);
}




/**************************************************************************************************/
int make_shortcut_plane(int IP, int ID, float rho[MAX_PIX][MAX_PIX][MAX_PIX], float origin[3], 
						float conlevel, float intlevel, float position,
						int maxNP[3], float Dmin, float Dmax)
{
	int box_point[3],box[3], box_min[3], box_shift[3];
	int r[3][4]; 
	GRIDCELL rt;
	int from[2],to[2];
	int square_plane;
	int I1,I2,I3;
	int II1,II2,II3;
	int i,j,m,km,jm;
	float v[3][5],vx[3][5];

	box_point[0] = maxNP[0]+1; 
	box_point[1] = maxNP[1]+1; 
	box_point[2] = maxNP[2]+1;
	box[0] = (maxNP[0] + 1); 
	box[1] = (maxNP[1] + 1); 
	box[2] = (maxNP[2] + 1);
	box_min[0] = 0; box_min[1] = 0; box_min[2] = 0;

	for (i=0; i<3; i++) {
		box_shift[i] = (int)(box_point[i]*origin[i]);
	}

//-------------------------------------------------------
//  ID = 0  ---  (100) plane;
//  ID = 1  ---  (010) plane;
//  ID = 2  ---  (001) plane;
//-------------------------------------------------------
	switch(ID)
	{
//------------------------------------------------------- (100)
	case(0):{
		r[0][0] = 0; r[1][0] = 0; r[2][0] = 0;
		r[0][1] = 0; r[1][1] = 1; r[2][1] = 0;
		r[0][2] = 0; r[1][2] = 1; r[2][2] = 1;
		r[0][3] = 0; r[1][3] = 0; r[2][3] = 1;
		I1 = (int)(position*maxNP[0]);
		II1 = I1;
		from[0] = box_min[2];
		  to[0] = box_min[2] + box[2] - 1;
		from[1] = box_min[1];
		  to[1] = box_min[1] + box[1] - 1;
		break;
			}
//------------------------------------------------------- (010)
	case(1):{
		r[0][0] = 0; r[1][0] = 0; r[2][0] = 0;
		r[0][1] = 1; r[1][1] = 0; r[2][1] = 0;
		r[0][2] = 1; r[1][2] = 0; r[2][2] = 1;
		r[0][3] = 0; r[1][3] = 0; r[2][3] = 1;
		I2 = (int)(position*maxNP[1]);
		II2 = I2;
		from[0] = box_min[2];
		  to[0] = box_min[2] + box[2] - 1;
		from[1] = box_min[0];
		  to[1] = box_min[0] + box[0] - 1;
		break;
			}
//------------------------------------------------------- (001)
	case(2):{
		r[0][0] = 0; r[1][0] = 0; r[2][0] = 0;
		r[0][1] = 1; r[1][1] = 0; r[2][1] = 0;
		r[0][2] = 1; r[1][2] = 1; r[2][2] = 0;
		r[0][3] = 0; r[1][3] = 1; r[2][3] = 0;
		I3 = (int)(position*maxNP[2]);
		II3 = I3;
		from[0] = box_min[1];
		  to[0] = box_min[1] + box[1] - 1;
		from[1] = box_min[0];
		  to[1] = box_min[0] + box[0] - 1;
		break;
			}
	}
	total_polygons = 0;
	for (j=from[0]; j<to[0]; j++){
		for (i=from[1]; i<to[1]; i++){
			for (m=0; m<4; m++){
				switch(ID)
				{
				case(0):{
					I2 = i + r[1][m] + box_shift[1];
					I3 = j + r[2][m] + box_shift[2];
					II2 = I2;
					II3 = I3;
					if (I2 >= box_point[1]) I2 = I2 - box_point[1]+1;
					if (I3 >= box_point[2]) I3 = I3 - box_point[2]+1;
					break;
						}
				case(1):{
					I1 = i + r[0][m] + box_shift[0];
					I3 = j + r[2][m] + box_shift[2];
					II1 = I1;
					II3 = I3;
					if (I1 >= box_point[0]) I1 = I1 - box_point[0]+1;
					if (I3 >= box_point[2]) I3 = I3 - box_point[2]+1;
					break;
						}
				case(2):{
					I1 = i + r[0][m] + box_shift[0];
					I2 = j + r[1][m] + box_shift[1];
					II1 = I1;
					II2 = I2;
					if (I1 >= box_point[0]) I1 = I1 - box_point[0]+1;
					if (I2 >= box_point[1]) I2 = I2 - box_point[1]+1;
					break;
						}
				}
				rt.p[m].x = (float) II1; 
				rt.p[m].y = (float) II2; 
				rt.p[m].z = (float) II3; 
				if (rho[I1][I2][I3] < 0) rt.val[m] = -rho[I1][I2][I3];
				else rt.val[m] = rho[I1][I2][I3];
			}
			square_plane = polygonize_plane(rt,conlevel,intlevel,Dmin,Dmax);
			if (square_plane > 0){
				for (m=0; m<square_plane; m++){                        // Loop all polygons
					plane[IP][total_polygons].nV = nVpolygon;
					for (km=0; km<nVpolygon; km++){                    // Loop all vertices
						plane[IP][total_polygons].sl.Index[km] = poly[m].Index[km];
						v[0][km] = (float)poly[m].p[km].x - 1;
						v[1][km] = (float)poly[m].p[km].y - 1;
						v[2][km] = (float)poly[m].p[km].z - 1;
						v[0][km] = (float)(v[0][km] / box_point[0]);   // Convert to grid fractional
						v[1][km] = (float)(v[1][km] / box_point[1]);   // Convert to grid fractional
						v[2][km] = (float)(v[2][km] / box_point[2]);   // Convert to grid fractional
						for (jm=0; jm<3; jm++){                        // Convert to orthogonal angstroms
							vx[jm][km] = f2o[jm][0]*v[0][km] + 
                                         f2o[jm][1]*v[1][km] + 
                                         f2o[jm][2]*v[2][km];
						} //--- jm loop
						plane[IP][total_polygons].sl.p[km].x = vx[0][km];
						plane[IP][total_polygons].sl.p[km].y = vx[1][km];
						plane[IP][total_polygons].sl.p[km].z = vx[2][km];
					} //--- km loop
					total_polygons++;
					if (total_polygons > MAXPOLYGON){
						printf("==================================================\n");
						printf("Number of polygons out of range ivert=%i\n\tMAXPOLYGON=%i\n",
							    total_polygons,MAXPOLYGON);
						exit(0);
						printf("==================================================\n");
					}
				} //--- m loop
			} //--- if (square_plane > 0)
		} //--- i loop
	} //--- j loop

	return(total_polygons);
}


//***************************************************************************//
void make_ordering()
{
	int i,j,I1;
	int nVector;
	XYZ A[12],C;
	int oIndex[12],Index[12],numIndex,ind;
	int Sign,sum;
	float vol;
	int dens[12];

	numIndex = 1;
	Index[0] = 0;
	ind = 0;
	while (numIndex < cPNum){
//-------------------------------------------------------------
		j = 0;
		for(i=0; i<cPNum; i++){
			if(i != ind){
				A[j].x = poly[0].p[i].x - poly[0].p[ind].x;
				A[j].y = poly[0].p[i].y - poly[0].p[ind].y;
				A[j].z = poly[0].p[i].z - poly[0].p[ind].z;
				oIndex[j] = i;
				j++;
			}
		}
		nVector = cPNum - 1;
//-------------------------------------------------------------
		for(i=0; i<nVector; i++){
			I1 = i;
			sum = 0;
			for(j=0; j<nVector; j++){
				if (j != I1){
					C.x = A[I1].y*A[j].z - A[I1].z*A[j].y;
					C.y = A[I1].z*A[j].x - A[I1].x*A[j].z;
					C.z = A[I1].x*A[j].y - A[I1].y*A[j].x;
					vol = C.x*Normal[0] + C.y*Normal[1] + C.z*Normal[2];
					if (vol >= 0) Sign = 1;
					else Sign = -1;
					sum += Sign;
				}
			}
			if (sum == (nVector-1)){
					Index[numIndex] = oIndex[I1];
					numIndex++;
					ind = oIndex[I1];
			}
		}
//-------------------------------------------------------------
	}
	for(i=0; i<cPNum; i++){
		A[i].x = poly[0].p[Index[i]].x;
		A[i].y = poly[0].p[Index[i]].y;
		A[i].z = poly[0].p[Index[i]].z;
		dens[i] = poly[0].Index[Index[i]];
	}

	for(i=0; i<cPNum; i++){
		poly[0].p[i].x = A[i].x;
		poly[0].p[i].y = A[i].y;
		poly[0].p[i].z = A[i].z;
		poly[0].Index[i] = dens[i];
	}
}

/**************************************************************************************************/
float get_isolevel(XYZ p, XYZ p1, XYZ p2, float valp1, float valp2, float level)
{
	float V;
	float D0,D1,D2;

	if (fabs(valp1-level) < 0.00001) valp1 = level;
	if (fabs(valp2-level) < 0.00001) valp2 = level;
	if (fabs(valp2-valp1) < 0.00001) return(valp1);

	D0 = (float)sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y) + (p2.z - p1.z)*(p2.z - p1.z));
	D1 = (float)sqrt((p.x - p1.x)*(p.x - p1.x) + (p.y - p1.y)*(p.y - p1.y) + (p.z - p1.z)*(p2.z - p1.z));
	D2 = D0 - D1;

	if (D1 < 0.00001) return (valp1);
	if (D2 < 0.00001) return (valp2);

	V = valp1 + (D1/D0) * (valp2 - valp1);
	if (V > level) return(V);

    return(level);
}

/**************************************************************************************************/
float side(XYZ P)
{
	float D;

	D = Normal[0]*Origin[0] + Normal[1]*Origin[1] + Normal[2]*Origin[2];
	return(Normal[0]*P.x + Normal[1]*P.y + Normal[2]*P.z - D);
}

/**************************************************************************************************/
void make_hkl_plane(GRIDCELL grid, float level, float intlevel,float Dmax,float Dmin )
{
	XYZ C,V,A,n,CA,CV,R;
	short v1[] = {0,1,3,0,4,5,7,4,0,1,2,3};
	short v2[] = {1,2,2,3,5,6,6,7,4,5,6,7};
	int i,j,skip;
	float CN,CM,K;
	float p_level;
	float den;

	n.x = Normal[0]; n.y = Normal[1]; n.z = Normal[2];
	A.x = Origin[0]; A.y = Origin[1]; A.z = Origin[2]; 
	cPNum = 0;
	for(i=0; i<12; i++){
		C.x = grid.p[v1[i]].x; //--- P1 point
		C.y = grid.p[v1[i]].y;
		C.z = grid.p[v1[i]].z;
		V.x = grid.p[v2[i]].x; //--- P2 point
		V.y = grid.p[v2[i]].y;
		V.z = grid.p[v2[i]].z;
		CA.x = A.x - C.x; 
		CA.y = A.y - C.y; 
		CA.z = A.z - C.z;
		CV.x = V.x - C.x; 
		CV.y = V.y - C.y; 
		CV.z = V.z - C.z;
		CN = CA.x * n.x + CA.y * n.y + CA.z * n.z;
		CM = CV.x * n.x + CV.y * n.y + CV.z * n.z;
//		if (CM != 0) 
			K = CN / CM;
//		else K = 1;
		if ((K >= 0) && (K <= 1)) {
			R.x = C.x + CV.x * K; 
			R.y = C.y + CV.y * K; 
			R.z = C.z + CV.z * K;
			skip = 0;
			for(j=0; j<cPNum; j++){
				if ((R.x == poly[0].p[j].x) && (R.y == poly[0].p[j].y) && (R.z == poly[0].p[j].z)) skip = 1;
			}
			if (skip == 0){
				p_level = (float)get_isolevel(R,C,V,grid.val[v1[i]],grid.val[v2[i]],level);
				den = p_level / (intlevel * (Dmax - Dmin));
				if (den > 1) den = 1;
				poly[0].Index[cPNum] = (int)(den * 99);
				poly[0].p[cPNum].x = R.x; poly[0].p[cPNum].y = R.y; poly[0].p[cPNum].z = R.z;
				cPNum++;
			}
		}
	}
    if (cPNum > 3) make_ordering();
}


/**************************************************************************************************/
int check_cube_with_plane(GRIDCELL grid)
{
	int cubeindex,i;

	cubeindex = 0;
	for (i=0; i<8; i++){
		if (side(grid.p[i]) > 0) cubeindex++; 
		if (side(grid.p[i]) < 0) cubeindex--; 
	}
	if (fabs(cubeindex) != 8) return(1);
	return (0);
}


/**************************************************************************************************/
void get_plane_normal_and_origin(short hkl[3], float pos, int nPoints[3], float cell[6])
{
	float x,y,z,x1,y1,z1;
	float A,B,C;
	XYZ OA,OB,OC,AB,AC,BC;
	float normal_l;
	int h,k,l;

	h = hkl[0];	k = hkl[1];	l = hkl[2];

//--------------- get orthogonal (HKL)
	if (h == 0) A = (float)nPoints[0]; else A = (float)nPoints[0] / (float)h;
	if (k == 0) B = (float)nPoints[1]; else B = (float)nPoints[1] / (float)k;
	if (l == 0) C = (float)nPoints[2]; else C = (float)nPoints[2] / (float)l;
	A = (A - 1) / (float)(nPoints[0] + 1);
	B = (B - 1) / (float)(nPoints[1] + 1);
	C = (C - 1) / (float)(nPoints[2] + 1);
//-------------------------------------------------------------- (a // h)
	x = A; y = 0; z = 0;
	x1 = x*f2o[0][0] + y*f2o[0][1] + z*f2o[0][2];
	y1 = x*f2o[1][0] + y*f2o[1][1] + z*f2o[1][2];
	z1 = x*f2o[2][0] + y*f2o[2][1] + z*f2o[2][2];
	hkl_o_axis[0][0] = (float)x1;
	hkl_o_axis[0][1] = (float)y1;
	hkl_o_axis[0][2] = (float)z1;
//-------------------------------------------------------------- (b // k)
	x = 0; y = B; z = 0;
	x1 = x*f2o[0][0] + y*f2o[0][1] + z*f2o[0][2];
	y1 = x*f2o[1][0] + y*f2o[1][1] + z*f2o[1][2];
	z1 = x*f2o[2][0] + y*f2o[2][1] + z*f2o[2][2];
	hkl_o_axis[1][0] = (float)x1;
	hkl_o_axis[1][1] = (float)y1;
	hkl_o_axis[1][2] = (float)z1;
//-------------------------------------------------------------- (c // l)
	x = 0; y = 0; z = C;
	x1 = x*f2o[0][0] + y*f2o[0][1] + z*f2o[0][2];
	y1 = x*f2o[1][0] + y*f2o[1][1] + z*f2o[1][2];
	z1 = x*f2o[2][0] + y*f2o[2][1] + z*f2o[2][2];
	hkl_o_axis[2][0] = (float)x1;
	hkl_o_axis[2][1] = (float)y1;
	hkl_o_axis[2][2] = (float)z1;

//--------------- get plane normal
	OA.x = hkl_o_axis[0][0];OA.y = hkl_o_axis[0][1];OA.z = hkl_o_axis[0][2];
	OB.x = hkl_o_axis[1][0];OB.y = hkl_o_axis[1][1];OB.z = hkl_o_axis[1][2];
	OC.x = hkl_o_axis[2][0];OC.y = hkl_o_axis[2][1];OC.z = hkl_o_axis[2][2];

	AB.x = OB.x - OA.x;
	AB.y = OB.y - OA.y;
	AB.z = OB.z - OA.z;

	AC.x = OC.x - OA.x;
	AC.y = OC.y - OA.y;
	AC.z = OC.z - OA.z;

	BC.x = OC.x - OB.x;
	BC.y = OC.y - OB.y;
	BC.z = OC.z - OB.z;
	if ((h != 0) && (k != 0) && (l != 0)){
		Normal[0] = AB.y * AC.z - AB.z * AC.y;
		Normal[1] = AB.z * AC.x - AB.x * AC.z;
		Normal[2] = AB.x * AC.y - AB.y * AC.x;
	}
	if ((h != 0) && (k != 0) && (l == 0)){
		Normal[0] = AB.y * OC.z - AB.z * OC.y;
		Normal[1] = AB.z * OC.x - AB.x * OC.z;
		Normal[2] = AB.x * OC.y - AB.y * OC.x;
	}
	if ((h != 0) && (k == 0) && (l != 0)){
		Normal[0] = OB.y * AC.z - OB.z * AC.y;
		Normal[1] = OB.z * AC.x - OB.x * AC.z;
		Normal[2] = OB.x * AC.y - OB.y * AC.x;
	}
	if ((h == 0) && (k != 0) && (l != 0)){
		Normal[0] = BC.y * OA.z - BC.z * OA.y;
		Normal[1] = BC.z * OA.x - BC.x * OA.z;
		Normal[2] = BC.x * OA.y - BC.y * OA.x;
	}
	if ((h != 0) && (k == 0) && (l == 0)){
		Normal[0] = OB.y * OC.z - OB.z * OC.y;
		Normal[1] = OB.z * OC.x - OB.x * OC.z;
		Normal[2] = OB.x * OC.y - OB.y * OC.x;
	}
	if ((h == 0) && (k != 0) && (l == 0)){
		Normal[0] = OC.y * OA.z - OC.z * OA.y;
		Normal[1] = OC.z * OA.x - OC.x * OA.z;
		Normal[2] = OC.x * OA.y - OC.y * OA.x;
	}
	if ((h == 0) && (k == 0) && (l != 0)){
		Normal[0] = OA.y * OB.z - OA.z * OB.y;
		Normal[1] = OA.z * OB.x - OA.x * OB.z;
		Normal[2] = OA.x * OB.y - OA.y * OB.x;
	}
	normal_l = (float)sqrt(Normal[0]*Normal[0] + Normal[1]*Normal[1] + Normal[2]*Normal[2]);
	Normal[0] /=  normal_l;
	Normal[1] /=  normal_l;
	Normal[2] /=  normal_l;

//--------------- get plane origin
	A = pos*(float)nPoints[0] - 1;
	B = pos*(float)nPoints[1] - 1;
	C = pos*(float)nPoints[2] - 1;
	x = A  / (float)(nPoints[0] + 1);
	y = B  / (float)(nPoints[1] + 1);
	z = C  / (float)(nPoints[2] + 1);
	x1 = x*f2o[0][0] + y*f2o[0][1] + z*f2o[0][2];
	y1 = x*f2o[1][0] + y*f2o[1][1] + z*f2o[1][2];
	z1 = x*f2o[2][0] + y*f2o[2][1] + z*f2o[2][2];
	Origin[0] = (float)x1;
	Origin[1] = (float)y1;
	Origin[2] = (float)z1;
}


/**************************************************************************************************/
int make_plane(int IP, short hkl[3], float rho[MAX_PIX][MAX_PIX][MAX_PIX], 
			           float conlevel, float intlevel, 
					   float position, int maxNP[3], 
					   float Dmin, float Dmax, float cell[6])
{
	int i,j,k,m;
	int I1,I2,I3;
	int r[3][8]; 
	GRIDCELL rt;
	float x,y,z;

	position = position*0.9999f;
	get_plane_normal_and_origin(hkl,position,maxNP,cell);

	r[0][0] = 0; r[1][0] = 0; r[2][0] = 0;
	r[0][1] = 1; r[1][1] = 0; r[2][1] = 0;
	r[0][2] = 1; r[1][2] = 1; r[2][2] = 0;
	r[0][3] = 0; r[1][3] = 1; r[2][3] = 0;
	r[0][4] = 0; r[1][4] = 0; r[2][4] = 1;
	r[0][5] = 1; r[1][5] = 0; r[2][5] = 1;
	r[0][6] = 1; r[1][6] = 1; r[2][6] = 1;
	r[0][7] = 0; r[1][7] = 1; r[2][7] = 1;

	total_polygons = 0;
/// Loop boxes
	for (k=0; k<maxNP[2]; k++){
		for (j=0; j<maxNP[1]; j++){
			for (i=0; i<maxNP[0]; i++){
				for (m=0; m<8; m++){
					I1 = i + r[0][m];
					I2 = j + r[1][m];
					I3 = k + r[2][m];
					x = (float)I1 - 1;
					y = (float)I2 - 1;
					z = (float)I3 - 1;
					x = x / (maxNP[0] + 1);
					y = y / (maxNP[1] + 1);
					z = z / (maxNP[2] + 1);
					rt.p[m].x = x*f2o[0][0] + y*f2o[0][1] + z*f2o[0][2];
					rt.p[m].y = x*f2o[1][0] + y*f2o[1][1] + z*f2o[1][2];
					rt.p[m].z = x*f2o[2][0] + y*f2o[2][1] + z*f2o[2][2];
					if (rho[I1][I2][I3] < 0) rt.val[m] = -1*rho[I1][I2][I3];
					else rt.val[m] = rho[I1][I2][I3];
				} //--- m loop
				if (check_cube_with_plane(rt) == 1) {
					cPNum = 0;
					make_hkl_plane(rt,conlevel,intlevel,Dmax,Dmin);
					if ( cPNum > 2){
						plane[IP][total_polygons].nV = cPNum;
						for(m=0; m<cPNum; m++){
							plane[IP][total_polygons].sl.Index[m] = poly[0].Index[m];
							plane[IP][total_polygons].sl.p[m].x = poly[0].p[m].x;
							plane[IP][total_polygons].sl.p[m].y = poly[0].p[m].y;
							plane[IP][total_polygons].sl.p[m].z = poly[0].p[m].z;
						}//--- m loop
						total_polygons++;
						if (total_polygons > MAXPOLYGON){
							printf("==================================================\n");
							printf("Number of polygons out of range ivert=%i\n\tMAXPOLYGON=%i\n",
								    total_polygons,MAXPOLYGON);
							return(0);
							printf("==================================================\n");
						}
					}
				}
			}
		}
	}
	return(total_polygons);
}


