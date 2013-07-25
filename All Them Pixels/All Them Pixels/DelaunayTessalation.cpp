#include "DelaunayTessalation.h"

bool DelaunayTessalation::containsPoint(Vector2f A, float a_x_2, float a_y_2, Vector2f B, float b_x_2, float b_y_2, Vector2f C, float c_x_2, float c_y_2, Vector2f point, float point_x_2, float point_y_2)
{
	float a = A.x - point.x;
	float b = A.y - point.y;
	float c = (a_x_2 - point_x_2) + (a_y_2 - point_y_2);
	float d = B.x - point.x;
	float e = B.y - point.y;
	float f = (b_x_2 - point_x_2) + (b_y_2 - point_y_2);
	float g = C.x - point.x;
	float h = C.y - point.y;
	float i = (c_x_2 - point_x_2) + (c_y_2 - point_y_2);

	return (a*e*i + b*f*g + c*d*h - c*e*g - b*d*i - a*f*h) > 0;
}

bool DelaunayTessalation::interssects(Vector2f a, Vector2f b, Vector2f c, Vector2f d)
{
	Vector2f r = b - a;
	Vector2f s = d - c;

	float numerator_t;
	float numerator_u = Vector2fMath::cross((c-a), r);
	float denominator = Vector2fMath::cross(r, s);
	float t;
	float u;

	if (denominator == 0.0f && numerator_u == 0.0f) //Collinear
	{
		return ((a.x <= c.x && c.x <= b.x) || (a.x <= d.x && d.x <= b.x) || (c.x < a.x && b.x < d.x))
			&& ((a.y <= c.y && c.y <= b.y) || (a.y <= d.y && d.y <= b.y) || (c.y < a.y && b.y < d.y));
	}

	if (denominator == 0.0f) // Parallel
	{
		return false;
	}
	
	
	numerator_t = Vector2fMath::cross((c-a), s);
	t = numerator_t / denominator;
	u = numerator_u / denominator;

	if (t == 1 || u == 1)
	{
		bool gröt = true;
	}
	
	return 0 <= t && t <= 1 && 0 <= u && u <= 1;
}

bool DelaunayTessalation::isLeftOfLine(Vector2f a, Vector2f b, Vector2f c)
{
	return Vector2fMath::cross(b - a, c - a) > 0;
}

bool DelaunayTessalation::sameSide(Vector2f a, Vector2f b, Vector2f c, Vector2f point)
{
	float cp1 = Vector2fMath::cross(point - c, a - c);
	float cp2 = Vector2fMath::cross(point - c, b - c);

	return cp1 * cp2 >= 0;
}

bool DelaunayTessalation::isInTriangle(Vector2f a, Vector2f b, Vector2f c, Vector2f point)
{
	return sameSide(a, b, c, point) && sameSide(a, c, b, point) && sameSide(a, point, b, c);
}

Vertex * DelaunayTessalation::getTriangles(Vertex * vertices, int count, int &triangleCount)
{
	int tessalationIndex = 0;
	Rect<float> boundingBox = Collision::getHitBox(vertices, count);
	list<vector<int>> triangles;
	vector<int> enclosingTriangle(3);
	Vector2f enclosingVertices[3];
	Vertex * tessalation;
	float * powf_x = new float[count];
	float * powf_y = new float[count];
	float * enc_powf_x = new float[3];
	float * enc_powf_y = new float[3];

	enclosingVertices[0] = Vector2f(boundingBox.left - 10, boundingBox.top - boundingBox.height - 20);
	enclosingVertices[1] = Vector2f(boundingBox.left + (2 * boundingBox.width) + 20, boundingBox.top + boundingBox.height + 10);
	enclosingVertices[2] = Vector2f(boundingBox.left - 10, boundingBox.top + boundingBox.height + 10);

	enclosingTriangle[0] = -1;
	enclosingTriangle[1] = -2;
	enclosingTriangle[2] = -3;

	triangles.push_back(enclosingTriangle);

	for (int i = 0; i < 3; i++)
	{
		enc_powf_x[i] = enclosingVertices[i].x * enclosingVertices[i].x;
		enc_powf_y[i] = enclosingVertices[i].y * enclosingVertices[i].y;
	}

	for (int i = 0; i < count; i++)
	{
		powf_x[i] = vertices[i].position.x * vertices[i].position.x;
		powf_y[i] = vertices[i].position.y * vertices[i].position.y;
	}
	int bahhhh = 0;
	for (int i = 0; i < count; i++)
	{
		list<vector<int>> edges;
		list<int> nodes;

		for (list<vector<int>>::iterator triangle = triangles.begin(); triangle != triangles.end();)
		{
			int a = (*triangle)[0];
			int b = (*triangle)[1];
			int c = (*triangle)[2];
			float a_x_2 = (a >= 0) ? powf_x[a] : enc_powf_x[-a - 1];
			float a_y_2 = (a >= 0) ? powf_y[a] : enc_powf_y[-a - 1];
			float b_x_2 = (b >= 0) ? powf_x[b] : enc_powf_x[-b - 1];
			float b_y_2 = (b >= 0) ? powf_y[b] : enc_powf_y[-b - 1];
			float c_x_2 = (c >= 0) ? powf_x[c] : enc_powf_x[-c - 1];
			float c_y_2 = (c >= 0) ? powf_y[c] : enc_powf_y[-c - 1];
			float point_x_2 = powf_x[i];
			float point_y_2 = powf_y[i];
			Vector2f A = (a >= 0) ? vertices[a].position : enclosingVertices[-a - 1];
			Vector2f B = (b >= 0) ? vertices[b].position : enclosingVertices[-b - 1];
			Vector2f C = (c >= 0) ? vertices[c].position : enclosingVertices[-c - 1];

			if (containsPoint(A, a_x_2, a_y_2, B, b_x_2, b_y_2, C, c_x_2, c_y_2, vertices[i].position, point_x_2, point_y_2))
			{
				vector<int> edgeA(2);
				vector<int> edgeB(2);
				vector<int> edgeC(2);

				edgeA[0] = a;
				edgeA[1] = b;
				edgeB[0] = b;
				edgeB[1] = c;
				edgeC[0] = c;
				edgeC[1] = a;

				edges.push_back(edgeA);
				edges.push_back(edgeB);
				edges.push_back(edgeC);

				nodes.push_back(a);
				nodes.push_back(b);
				nodes.push_back(c);

				bahhhh++;

				triangle = triangles.erase(triangle);
			}
			else
			{
				triangle++;
			}
		}
		int ba = 0;
		for (auto edge : edges)
		{
			bool edgeInterssects = false;

			for (auto node : nodes)
			{
				int a = edge[0];
				int b = edge[1];
				int c = node;

				if (c != a && c != b)
				{
					Vector2f A = (a >= 0) ? vertices[a].position : enclosingVertices[(-a) - 1];
					Vector2f B = (b >= 0) ? vertices[b].position : enclosingVertices[(-b) - 1];
					Vector2f C = (c >= 0) ? vertices[c].position : enclosingVertices[(-c) - 1];


					if (interssects(A, B, C, vertices[i].position))
					{
						edgeInterssects = true;

						break;
					}
				}
			}

			if (!edgeInterssects)
			{	
				int a = edge[0];
				int b = edge[1];
				vector<int> triangle(3);
				Vector2f A = (a >= 0) ? vertices[a].position : enclosingVertices[(-a) - 1];
				Vector2f B = (b >= 0) ? vertices[b].position : enclosingVertices[(-b) - 1];
				Vector2f C = vertices[i].position;

				if (isLeftOfLine(A, B, C))
				{
					triangle[0] = a;
					triangle[1] = b;
					triangle[2] = i;
				}
				else
				{
					triangle[0] = b;
					triangle[1] = a;
					triangle[2] = i;
				}		
				ba++;

				triangles.push_back(triangle);
			}
		}
		if (ba != bahhhh)
		{
			bool gröt = true;
		}
	}

	// Cleanup
	for (list<vector<int>>::iterator triangle = triangles.begin(); triangle != triangles.end();)
	{
		int a = (*triangle)[0];
		int b = (*triangle)[1];
		int c = (*triangle)[2];

		if (a < 0 || b < 0 || c < 0)
		{
			triangle = triangles.erase(triangle);
		}
		else
		{
			triangle++;
		}
	}
	delete[] powf_x;
	delete[] powf_y;
	delete[] enc_powf_x;
	delete[] enc_powf_y;

	// Build concave shape
	triangleCount = triangles.size();
	tessalation = new Vertex[triangleCount * 3];

	for (auto triangle : triangles)
	{
		tessalation[tessalationIndex++] = vertices[triangle[0]];
		tessalation[tessalationIndex++] = vertices[triangle[1]];
		tessalation[tessalationIndex++] = vertices[triangle[2]];
	}
	return tessalation;
}
