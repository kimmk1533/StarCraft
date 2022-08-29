#pragma once

namespace CoreEngine
{
	class AStar final
	{
	private:
		class AStarNode final
		{
		public:
			D3DXVECTOR3 Position;
			READONLY_PROPERTY(uint32_t, F); // G + H (전체 거리)
			GET(F) { return G + H; }
			GET(F) const { return G + H; }
			uint32_t G; // 시작점부터의 거리
			uint32_t H; // 도착점까지의 거리

		public:
			AStarNode();
			virtual ~AStarNode();

			bool operator<(const AStarNode& _other) const
			{
				return this->F > _other.F;
			}
		};

	private:
		static uint32_t ApproximateDistXY(const uint32_t& _x1, const uint32_t& _y1, const uint32_t& _x2, const uint32_t& _y2);
		static uint32_t ApproximateDistXY(const D3DXVECTOR3& _pos1, const D3DXVECTOR3& _pos2);
		static uint32_t ApproximateDistXY(const D3DXVECTOR3* _pos1, const D3DXVECTOR3* _pos2);

	public:
		static std::vector<D3DXVECTOR3> AStarFindPath(bool** _walkability, const uint32_t& _width, const uint32_t& _height, const uint32_t& _startX, const uint32_t& _startY, const uint32_t& _endX, const uint32_t& _endY);

	private:
		AStar() = delete;
		virtual ~AStar() = delete;

	};
}