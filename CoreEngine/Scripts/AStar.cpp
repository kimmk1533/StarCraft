#include "stdafx_Core.h"
#include "AStar.h"

namespace CoreEngine
{
	uint32_t AStar::ApproximateDistXY(const uint32_t& _x1, const uint32_t& _y1, const uint32_t& _x2, const uint32_t& _y2)
	{
		uint32_t dx = abs((long)_x1 - (long)_x2);
		uint32_t dy = abs((long)_y1 - (long)_y2);

		uint32_t e1 = abs((long)dx - (long)dy);
		uint32_t e2 = min(dx, dy);

		return e1 * 10 + e2 * 14;
	}
	uint32_t AStar::ApproximateDistXY(const D3DXVECTOR3& _pos1, const D3DXVECTOR3& _pos2)
	{
		return ApproximateDistXY(_pos1.x, _pos1.y, _pos2.x, _pos2.y);
	}
	uint32_t AStar::ApproximateDistXY(const D3DXVECTOR3* _pos1, const D3DXVECTOR3* _pos2)
	{
		return ApproximateDistXY(_pos1->x, _pos1->y, _pos2->x, _pos2->y);
	}

	std::vector<D3DXVECTOR3> AStar::AStarFindPath(bool** _walkability, const uint32_t& _width, const uint32_t& _height, const uint32_t& _startX, const uint32_t& _startY, const uint32_t& _endX, const uint32_t& _endY)
	{
		std::vector<D3DXVECTOR3> result;
		std::priority_queue<AStarNode> openNodes;
		AStarNode currentNode;

		// 시작점부터의 거리(A* 에서의 f)와
		// 부모 노드의 방향을 기록할 맵 초기화
		std::vector<std::vector<uint32_t>> gMap(_height, std::vector<uint32_t>(_width, -1));
		std::vector<std::vector<uint8_t>> dirMap(_height, std::vector<uint8_t>(_width, 8));

		// 시작점 넣기
		currentNode.Position = D3DXVECTOR3(_startX, _startY, 0.0f);
		currentNode.G = ApproximateDistXY(_startX, _startY, _endX, _endY);
		currentNode.H = 0;
		openNodes.push(currentNode);
		gMap[_startY][_startX] = 0;

		// 8방향에 대한 x, y 변위 배열
		static const int8_t direction_x[] = { 1, 1, 0, -1, -1, -1, 0, 1 };
		static const int8_t direction_y[] = { 0, -1, -1, -1, 0, 1, 1, 1 };

		int32_t x, y, nx, ny;

		// A* algorithm
		while (!openNodes.empty())
		{
			currentNode = openNodes.top(); openNodes.pop();
			x = currentNode.Position.x;
			y = currentNode.Position.y;

			// old g value
			uint32_t oldG = gMap[y][x];

			if (x == _endX && y == _endY)
			{
				nx = x;
				ny = y;

				uint8_t	current_direction = 8;
				while (!(nx == _startX && ny == _startY))
				{
					uint8_t next_direction = dirMap[ny][nx];

					// 방향이 바뀔때만 path에 추가.
					if (current_direction != next_direction)
						result.push_back(D3DXVECTOR3(nx, ny, 0.0f));

					nx += direction_x[next_direction];
					ny += direction_y[next_direction];
					current_direction = next_direction;
				}

				return result;
			}

			// 인접 노드 탐색
			for (uint8_t direction = 0; direction < 8; ++direction)
			{
				nx = x + direction_x[direction];
				ny = y + direction_y[direction];

				// 유효하지 않은 범위
				if (nx < 0 || nx >= _width ||
					ny < 0 || ny >= _height)
					continue;

				// 통행 불가
				if (!_walkability[ny][nx])
					continue;

				uint32_t distance = 10;

				// 대각선인 경우
				if (direction % 2 == 1)
					distance += 4;

				// 인접 칸이 막힌 경우
				if (_walkability[ny][x] == false || _walkability[y][nx] == false)
					continue;

				// h value(distance from end point)
				uint32_t h = ApproximateDistXY(nx, ny, _endX, _endY);

				// new g value(distance from start point)
				uint32_t newG = oldG + distance;

				// h value는 변하지 않으므로 g value만 기존 값과 비교하면 됨.
				// 더 작은 경우 open node에 새로 넣기.priority 는 f로 설정
				if (newG < gMap[ny][nx])
				{
					gMap[ny][nx] = newG;

					currentNode.Position = D3DXVECTOR3(nx, ny, 0.0f);
					currentNode.G = gMap[ny][nx];
					currentNode.H = h;
					openNodes.push(currentNode);

					// 부모 노드로의 방향은 지금 방향을 거꾸로 하면 됨.
					dirMap[ny][nx] = (direction + 4) % 8;
				}
			}
		}

		// 경로 존재 X
		//t2 = time.time()
		//print("No path. at: " + str(t2 - t1) + " sec")

		return result;
	}

	AStar::AStarNode::AStarNode()
	{
		Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		G = 0;
		H = 0;
	}
	AStar::AStarNode::~AStarNode()
	{
	}
}