#include <vector>
#include <iostream>
#include <algorithm>
#include <list>
using std::cout;
using std::endl;
using std::find;
using std::iterator;
using std::list;
using std::vector;

void FIFO(vector<int> &ref_string, int num_page_frames);
void LRU(vector<int> &ref_string, int num_page_frames);
void OPT(vector<int> &ref_string, int num_page_frames);
int FindPageUsedLast(vector<int> &ref_string, list<int> &opt_list, int pos_in_ref_string);

int main()
{
  int num_page_frames = 4;
  vector<int> ref_string = {0, 1, 7, 2, 3, 2, 7, 1, 0, 3};
  //vector<int>ref_string = {1, 2, 3, 4, 2, 1, 5, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6};
  
  FIFO(ref_string, num_page_frames);
  LRU(ref_string, num_page_frames);
  OPT(ref_string, num_page_frames);

  num_page_frames = 8;
  
  FIFO(ref_string, num_page_frames);
  LRU(ref_string, num_page_frames);
  OPT(ref_string, num_page_frames);

  return 0;
}

void FIFO(vector<int> &ref_string, int num_page_frames)
{
  list<int> fifo_queue;
  int page_faults = 0;

  for (int page_number : ref_string)
  {
    auto iter = find(fifo_queue.begin(), fifo_queue.end(), page_number);

    if (iter == fifo_queue.end())
    {
      page_faults++;
      fifo_queue.push_front(page_number);
      if (fifo_queue.size() > num_page_frames)
      {
        fifo_queue.pop_back(); // Remove oldest
      }
    }
  }

  cout << "Using FIFO with " << num_page_frames << " page frames yielded " << page_faults << " page faults.\n";
}

void LRU(vector<int> &ref_string, int num_page_frames)
{
  list<int> LRU_stack;
  int page_faults = 0;

  for (int page_number : ref_string)
  {
    auto iter = find(LRU_stack.begin(), LRU_stack.end(), page_number);

    if (iter != LRU_stack.end())
    {
      LRU_stack.erase(iter);
      LRU_stack.push_front(page_number);
    }
    else
    {
      page_faults++;
      LRU_stack.push_front(page_number);
      if (LRU_stack.size() > num_page_frames)
      {
        LRU_stack.pop_back();
      }
    }
  }

  cout << "Using LRU with " << num_page_frames << " page frames yielded " << page_faults << " page faults.\n";
}

void OPT(vector<int> &ref_string, int num_page_frames)
{
  list<int> opt_list;
  int page_faults = 0;

  for (int i = 0; i < ref_string.size(); i++)
  {
    int page_number = ref_string[i];
    auto iter = find(opt_list.begin(), opt_list.end(), page_number);

    if (opt_list.size() < num_page_frames && iter == opt_list.end())
    {
      opt_list.push_front(page_number);
      ++page_faults;
    }
    else if (opt_list.size() == num_page_frames && iter == opt_list.end())
    {
      ++page_faults;
      int page_used_last = FindPageUsedLast(ref_string, opt_list, i);
      auto it = find(opt_list.begin(), opt_list.end(), page_used_last);
      opt_list.erase(it);
      opt_list.push_front(page_number);
    }
  }

  cout << "Using OPT with " << num_page_frames << " page frames yielded " << page_faults << " page faults.\n";
}

int FindPageUsedLast(vector<int> &ref_string, list<int> &opt_list, int pos_in_ref_string)
{
  int found_page_number = -1;
  int found_distance = -1000000;

  for (auto it = opt_list.begin(); it != opt_list.end(); ++it)
  {
    int curr_page_number = *it;
    int curr_distance = 1000000;
    for (int i = pos_in_ref_string; i < ref_string.size(); ++i)
    {
      if (ref_string[i] == curr_page_number)
      {
        curr_distance = i - pos_in_ref_string;
        break;
      }
    }
    if (curr_distance > found_distance)
    {
      found_page_number = curr_page_number;
      found_distance = curr_distance;
    }
  }
  return found_page_number;
}