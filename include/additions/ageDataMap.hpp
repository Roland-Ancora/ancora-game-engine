//#################################################################//
// 
//							DataMap
// 
//		The DataMap is class that contains dynamic map of specified
//	type elements, consisting of square segments.
// 
//		The DataMap module has external dependencies: STD.
// 
// 
//#################################################################//



#pragma once
#include <math.h>
#include <iostream>



enum segments_sides { SEGMENT_TOP, SEGMENT_RIGHT, SEGMENT_BOTTOM, SEGMENT_LEFT };

template <class ItemT>
class DataMap {
	unsigned int segment_size;
	ItemT** map;
	DataMap<ItemT>* segments_near[4] { 0, 0, 0, 0 };
private:
	void clear_segments_near(DataMap<ItemT>* from_segment);
public:
	DataMap(unsigned int segment_map_size);
public:
	int get_segment_size() const { return segment_size; }
	DataMap<ItemT>* get_segment(segments_sides side) const { return segments_near[side]; }
public:
	void create_segments(unsigned int count, segments_sides dir);
	void delete_other_segments();
	void resize_origin_segment(unsigned int segment_map_size);
public:
	ItemT* operator() (int x, int y);
	ItemT* operator() (float x, float y);
	ItemT* operator() (double x, double y);
};





template <class ItemT>
DataMap<ItemT>::DataMap(unsigned int segment_map_size)
	: segment_size(segment_map_size)
{
	if (segment_map_size % 2 == 0) {
		printf("Can't create DataMap with even number.\n");
		return;
	}
	map = new ItemT*[segment_map_size];
	for (int i = 0; i < segment_map_size; i++)
		map[i] = new ItemT[segment_map_size];
}

template <class ItemT>
ItemT* DataMap<ItemT>::operator() (int x, int y)
{ 
	int count_elements_on_side = (segment_size - 1) / 2;
	if (abs(x) > count_elements_on_side || abs(y) > count_elements_on_side) {
		segments_sides x_segments_step_dir, y_segments_step_dir;
		if (x > 0)
			x_segments_step_dir = SEGMENT_RIGHT;
		else
			x_segments_step_dir = SEGMENT_LEFT;
		if (y > 0)
			y_segments_step_dir = SEGMENT_TOP;
		else
			y_segments_step_dir = SEGMENT_BOTTOM;
		int x_segments_step = (x + count_elements_on_side) / segment_size;
		int y_segments_step = (y + count_elements_on_side) / segment_size;
		DataMap<ItemT>* now_working = this;
		for (int i = 0; i < x_segments_step; i++) {
			if (now_working->segments_near[x_segments_step_dir] == 0)
				now_working->segments_near[x_segments_step_dir] = new DataMap<ItemT>(segment_size);
			now_working = now_working->segments_near[x_segments_step_dir];
		}
		for (int i = 0; i < y_segments_step; i++) {
			if (now_working->segments_near[y_segments_step_dir] == 0)
				now_working->segments_near[y_segments_step_dir] = new DataMap<ItemT>(segment_size);
			now_working = now_working->segments_near[y_segments_step_dir];
		}
		int x_finaly_segment_center = x_segments_step * segment_size;
		int y_finaly_segment_center = y_segments_step * segment_size;
		int x_finaly_elemnts_in_side = x_finaly_segment_center - x;
		int y_finaly_elemnts_in_side = y_finaly_segment_center - y;
		int finaly_x_id, finaly_y_id;
		if (x > 0)
			finaly_x_id = count_elements_on_side - x_finaly_elemnts_in_side;
		else
			finaly_x_id = count_elements_on_side + x_finaly_elemnts_in_side;
		if (y > 0)
			finaly_y_id = count_elements_on_side - y_finaly_elemnts_in_side;
		else
			finaly_y_id = count_elements_on_side + y_finaly_elemnts_in_side;
		return (*now_working)(finaly_x_id, finaly_y_id);
	}
	else
		return &map[count_elements_on_side + x][count_elements_on_side + y];
}

template <class ItemT>
ItemT* DataMap<ItemT>::operator() (float x, float y)
{
	return (*this)(int(floor(x)), int(floor(y)));
}

template <class ItemT>
ItemT* DataMap<ItemT>::operator() (double x, double y)
{
	return (*this)(int(floor(x)), int(floor(y)));
}

template <class ItemT>
void DataMap<ItemT>::create_segments(unsigned int count, segments_sides dir)
{
	DataMap<ItemT>* now_working = this;
	for (unsigned int i = 0; i < count; i++) {
		now_working->segments_near[dir] = new DataMap<ItemT>(segment_size);
		now_working = now_working->segments_near[dir];
	}
}

template <class ItemT>
void DataMap<ItemT>::clear_segments_near(DataMap<ItemT>* from_segment)
{
	for (int i = 0; i < 4; i++) {
		if (segments_near[i] != 0 && segments_near[i] != from_segment)
			clear_segments_near();
	}
	for (unsigned int l = 0; l < segment_size; l++)
		delete[] map[l];
	delete[] map;
}

template <class ItemT>
void DataMap<ItemT>::delete_other_segments()
{
	for (int i = 0; i < 4; i++)
		if (segments_near[i] != 0)
			clear_segments_near(this);
	for (int i = 0; i < 4; i++)
		segments_near[i] = 0;
}

template <class ItemT>
void DataMap<ItemT>::resize_origin_segment(unsigned int segment_map_size)
{
	if (segment_map_size % 2 == 0) {
		printf("Can't resize segment with even number.\n");
		return;
	}
	for (int i = 0; i < 4; i++)
		if (segments_near[i] != 0) {
			printf("Can't resize the origin segment: it has segments near\n");
			return;
		}
	for (unsigned int l = 0; l < segment_size; l++)
		delete[] map[l];
	delete[] map;
	map = new ItemT * [segment_map_size];
	for (int i = 0; i < segment_map_size; i++)
		map[i] = new ItemT[segment_map_size];
	segment_size = segment_map_size;
}
