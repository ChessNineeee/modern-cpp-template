//
// Created by wuzhengqi on 24-10-13.
//
#include <gtest/gtest.h>
#include <bits/stdc++.h>
#include <boost/asio.hpp>
using namespace std;

class DiffArray
{
public:
  DiffArray(const vector<int>& _arr) : arr(_arr) {}

  void update(int num, int closedLeft, int closedRight)
  {
    updateArrByNumAndIdx(num, closedLeft);
    updateArrByNumAndIdx(-num, closedRight + 1);
  }

  vector<int> res()
  {
    return prefixSums(arr);
  }
private:
  void updateArrByNumAndIdx(int num, int idx)
  {
    if (valid_at(idx))
    {
      arr.at(idx) += num;
    }
  }
  bool valid_at(int idx)
  {
    if (idx < 0 || idx >= arr.size())
    {
      return false;
    }
    return true;
  }
  vector<int> prefixSums(const vector<int>& arr)
  {
    auto res = vector<int>();
    inclusive_scan(cbegin(arr), cend(arr), back_inserter(res), plus<>{});
    return res;
  }
  vector<int> arr;
};

struct ArithmeticProgression
{
  int first;
  int last;
  int d;
};

struct UpdateOpPair
{
  int num;
  int idx;
};

struct UpdateInterval
{
  int closedLeft;
  int closedRight;

  vector<UpdateOpPair> getUpdateOps(const ArithmeticProgression& prog) const
  {
    return vector<UpdateOpPair>{
      UpdateOpPair{.num = prog.first, .idx = closedLeft},
      UpdateOpPair{.num = -prog.first + prog.d, .idx = closedLeft + 1},
      UpdateOpPair{.num = -prog.last - prog.d, .idx = closedRight + 1},
      UpdateOpPair{.num = prog.last, .idx = closedRight + 2},
    };
  }
};

class ArithmeticDiffArray
{
 public:
  ArithmeticDiffArray(const vector<int>& _arr) : arr(_arr) {}
  void update(const ArithmeticProgression& prog, const UpdateInterval& interval)
  {
    const auto updateOps = interval.getUpdateOps(prog);
    for (const auto& updateOp : updateOps)
    {
      updateArrByNumAndIdx(updateOp.num, updateOp.idx);
    }
  }

  vector<int> res()
  {
    return prefixSums(prefixSums(arr));
  }
private:
  void updateArrByNumAndIdx(int num, int idx)
  {
    if (valid_at(idx))
    {
      arr.at(idx) += num;
    }
  }
  bool valid_at(int idx)
  {
    if (idx < 0 || idx >= arr.size())
    {
      return false;
    }
    return true;
  }
  vector<int> prefixSums(const vector<int>& arr)
  {
    auto res = vector<int>();
    inclusive_scan(cbegin(arr), cend(arr), back_inserter(res), plus<>{});
    return res;
  }
  vector<int> arr;
};

template <class T>
void print_vector(const vector<T>& v)
{
  for_each(cbegin(v), cend(v), [](const T& i) { cout << i << " "; });
  cout << endl;
}

template <class T>
void print_matrix(const vector<vector<T>>& v)
{
  for_each(cbegin(v), cend(v), [](const vector<T>& i) { for_each(cbegin(i), cend(i), [](const T& j) { cout << j << "\t"; }); cout << endl; });
  cout << endl;
}

template <class T1, class T2>
void print_map(const map<T1, T2>& m)
{
  for_each(cbegin(m), cend(m), [](const pair<T1, T2>& p) { cout << "(" << p.first << ", " << p.second << ")"; });
  cout << endl;
}

class NumMatrix
{
public:
  NumMatrix(const vector<vector<int>>& matrix): m_oldMatrix(matrix)
  {
    updatePrefixSums();
  }

  int sumRegion(int row1, int col1, int row2, int col2)
  {
    auto lowerRight = getPrefixSumByLineAndCol(getIdx(row2), getIdx(col2));
    auto left = getPrefixSumByLineAndCol(getIdx(row2), getIdx(col1-1));
    auto up = getPrefixSumByLineAndCol(getIdx(row1-1), getIdx(col2));
    auto compensate = getPrefixSumByLineAndCol(getIdx(row1-1), getIdx(col1-1));
    return lowerRight.value_or(0) - left.value_or(0) - up.value_or(0) + compensate.value_or(0);
  }

  vector<vector<int>> getPrefixSums()
  {
    return m_prefixSums;
  }

private:
  int getIdx(int number) { return number; }
  void updatePrefixSums()
  {
    initEmptyMatrixByOld();
    for (int i = 0; i < m_oldMatrix.size(); i++)
    {
      updatePrefixSumsByLine(i);
    }
  }

  void initEmptyMatrixByOld()
  {
    m_prefixSums = vector(m_oldMatrix.size(), vector<int>(m_oldMatrix[0].size(), 0));
  }

  void updatePrefixSumsByLine(int i)
  {
    for (int j = 0; j < m_oldMatrix[i].size(); j++)
    {
      updatePrefixSumsByLineAndCol(i, j);
    }
  }

  void updatePrefixSumsByLineAndCol(int i, int j)
  {
    auto left = getPrefixSumByLineAndCol(i-1, j);
    auto up = getPrefixSumByLineAndCol(i, j-1);
    auto upperLeft = getPrefixSumByLineAndCol(i-1, j-1);
    auto cur = getNumByLineAndCol(i, j);
    m_prefixSums[i][j] = cur.value_or(0) + left.value_or(0) + up.value_or(0) - upperLeft.value_or(0);
  }

  optional<int> getNumByLineAndCol(int i, int j)
  {
    if (i < 0) return nullopt;
    if (j < 0) return nullopt;
    return m_oldMatrix[i][j];
  }

  optional<int> getPrefixSumByLineAndCol(int i, int j)
  {
    if (i < 0) return nullopt;
    if (j < 0) return nullopt;
    return m_prefixSums[i][j];
  }

  vector<vector<int>> m_prefixSums;
  const vector<vector<int>>& m_oldMatrix;
};

class DiffMatrix
{

};

class Solution2132
{
public:
  bool possibleToStamp(vector<vector<int>>& grid, int stampHeight, int stampWidth)
  {
    init(grid, stampHeight, stampWidth);
    run();
    return result();
  }


private:
  void init(const vector<vector<int>>& grid, int h, int w)
  {
    updateBasic(grid, h, w);
    updatePrefixSums();
    updateDiffArrayBasic();
  }

  void run()
  {
    for (int i = 0; i < grid.size(); i++)
    {
     updateDiffArrayByLine(i);
    }
    updateDiffArrayPrefixSum();
    updateResByDiffArray();
  }

  void updateResByDiffArray()
  {
    for (int i = 0; i < grid.size(); i++)
    {
      for (int j = 0; j < grid[i].size(); j++)
      {
        if (pointValidAt(i, j) && getDiffArrayNumByLineAndCol(i, j).value_or(0) == 0)
        {
          res = false;
          return;
        }
      }
    }
  }

  void updateDiffArrayPrefixSum()
  {
    for (int i = 0; i < grid.size(); i++)
    {
      updateDiffArrayPrefixSumByLine(i);
    }
  }

  void updateDiffArrayPrefixSumByLine(int i)
  {
    for (int j = 0; j < grid[0].size(); j++)
    {
      updateDiffArrayPrefixSumByLineAndCol(i, j);
    }
  }

  void updateDiffArrayPrefixSumByLineAndCol(int i, int j)
  {
    auto up = getDiffArrayByLineAndCol(i-1, j);
    auto left = getDiffArrayByLineAndCol(i, j-1);
    auto upperLeft = getDiffArrayByLineAndCol(i-1, j-1);
    auto curNum = getDiffArrayNumByLineAndCol(i, j);
    diffArray[i][j] = curNum.value_or(0) + up.value_or(0) + left.value_or(0) - upperLeft.value_or(0);
  }

  optional<int> getDiffArrayByLineAndCol(int x, int y)
  {
    if (validAt(x, y)) return prefixSums.at(x).at(y);
    return nullopt;
  }

  optional<int> getDiffArrayNumByLineAndCol(int x, int y)
  {
    if (validAt(x, y)) return grid.at(x).at(y);
    return nullopt;
  }

  void updateDiffArrayByLine(int i)
  {
    for (int j = 0; j < grid[i].size(); j++)
    {
      updateDiffArrayByPoint(i, j);
    }
  }

  void updateDiffArrayByPoint(int startX, int startY)
  {
    if (pointValidAt(startX, startY))
    {
      updateDiffArrayByRegionOps(newRegionByPoint(startX, startY));
    }
  }

  struct region
  {
    int startX, startY, endX, endY;
  };
  optional<region> newRegionByPoint(int startX, int startY)
  {
    if (validAt(startX + h - 1, startY + w -1))
    {
      return region {
        .startX = startX,
        .startY = startY,
        .endX = startX + h - 1,
        .endY = startY + w - 1,
      };
    }
    return nullopt;
  }

  void updateDiffArrayByRegionOps(optional<region> regionOps)
  {
    if (regionOps)
    {
      updateDiffArrayByRegion(regionOps.value());
    }
  }

  void updateDiffArrayByRegion(region region)
  {
    if (validIn(region))
    {
      incrDiffArrayAt(region.startX, region.startY);
      decrDiffArrayAt(region.endX+1, region.startY);
      decrDiffArrayAt(region.startX, region.endY + 1);
      incrDiffArrayAt(region.endX + 1, region.endY + 1);
    }
  }

  void incrDiffArrayAt(int x, int y)
  {
    if (validAt(x, y))
    {
      diffArray.at(x).at(y) += 1;
    }
  }

  void decrDiffArrayAt(int x, int y)
  {
    if (validAt(x, y))
    {
      diffArray.at(x).at(y) -= 1;
    }
  }
  bool validIn(const region& region)
  {
    return sumRegion(region) == 0;
  }

  int sumRegion(const region& region)
  {
    auto left = getPrefixSumByLineAndCol(region.endX, region.startY - 1);
    auto up = getPrefixSumByLineAndCol(region.startX - 1, region.endY);
    auto upperLeft = getPrefixSumByLineAndCol(region.startX - 1, region.startY - 1);
    auto cur = getPrefixSumByLineAndCol(region.endX, region.endY);
    return cur.value_or(0) - left.value_or(0) - up.value_or(0) + upperLeft.value_or(0);
  }
  bool pointValidAt(int x, int y)
  {
    return getNumByLineAndCol(x, y).value_or(1) == 0;
  }

  bool result()
  {
    return res;
  }

  void updateBasic(const vector<vector<int>>& _grid, int _h, int _w)
  {
    grid = _grid;
    h = _h;
    w = _w;
  }

  void updatePrefixSums()
  {
    initPrefixSumsByGrid();
    for (int i = 0; i < grid.size(); i++)
    {
      updatePrefixSumsByLine(i);
    }
  }

  void updateDiffArrayBasic()
  {
    initDiffArrayByGrid();
  }

  void initDiffArrayByGrid()
  {
    diffArray = vector<vector<int>>(grid.size(), vector<int>(grid[0].size(), 0));
  }

  void initPrefixSumsByGrid()
  {
    prefixSums = vector<vector<int>>(grid.size(), vector<int>(grid[0].size(), 0));
  }

  void updatePrefixSumsByLine(int i)
  {
    for (int j = 0; j < grid[0].size(); j++)
    {
      updatePrefixSumsByLineAndCol(i, j);
    }
  }

  void updatePrefixSumsByLineAndCol(int i, int j)
  {
    auto up = getPrefixSumByLineAndCol(i-1, j);
    auto left = getPrefixSumByLineAndCol(i, j-1);
    auto upperLeft = getPrefixSumByLineAndCol(i-1, j-1);
    auto curNum = getNumByLineAndCol(i, j);
    prefixSums[i][j] = curNum.value_or(0) + up.value_or(0) + left.value_or(0) - upperLeft.value_or(0);
  }

  optional<int> getPrefixSumByLineAndCol(int x, int y)
  {
    if (validAt(x, y)) return prefixSums.at(x).at(y);
    return nullopt;
  }

  optional<int> getNumByLineAndCol(int x, int y)
  {
    if (validAt(x, y)) return grid.at(x).at(y);
    return nullopt;
  }

  bool validAt(int x, int y)
  {
    if (x < 0 || y < 0) return false;
    if (x >= grid.size() || y >= grid[0].size()) return false;
    return true;
  }
  vector<vector<int>> grid;
  int h;
  int w;

  vector<vector<int>> prefixSums;
  vector<vector<int>> diffArray;
  bool res = true;
};

TEST(TmpDiffArray, CheckValues)
{
  auto arr = DiffArray(vector<int>(9, 0));
  arr.update(3, 2, 5);
  arr.update(-2, 1, 6);
  arr.update(5, 4, 7);
  auto res = arr.res();
  print_vector<int>(res);
}

TEST(TmpArithmeticDiffArray, CheckValues)
{
  auto arr = ArithmeticDiffArray(vector<int>(9, 0));
  arr.update(ArithmeticProgression{4, 16, 3}, UpdateInterval{2, 6});
  auto res = arr.res();
  print_vector<int>(res);
}

TEST(TmpMatrixPrefixNum, CheckValues)
{
  auto matrix = NumMatrix(vector<vector<int>>{
    vector<int>{3,0,1,4,2},
    vector<int>{5,6,3,2,1},
    vector<int>{1,2,0,1,5},
    vector<int>{4,1,0,4,7},
    vector<int>{1,0,3,0,5},
  });
  cout << matrix.sumRegion(2,1,4,3) << endl;
  print_matrix(matrix.getPrefixSums());


}

TEST(Variant, BasicAPI)
{
  auto n = variant<vector<int>, map<int, int>>(vector<int>(1, 0));
  if (const auto * ptr = std::get_if<map<int, int>>(&n))
  {
    ASSERT_EQ(false, true);
  } else
  {
    ASSERT_EQ(ptr, nullptr);
  }
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}