#include "TableEdge.h"


TableEdge::TableEdge(const Vector3& a, const Vector3& b)
{
	edgeStart = a;
	edgeLength = b-a;
	float dy = b.y - a.y;
	float dx = b.x - a.x;
	Vector3 normalcoords(-dy, dx, 0.0f);
	Vector3 normalcoords1(dy, -dx, 0.0f);
	
	normalEdge = normalcoords1 - normalcoords;

	edgeNormal = Vector3(-dy, dx, 0.0f);
	edgeNormal.Normalise();

	edgeLengthUnit = edgeLength;
	edgeLengthUnit.Normalise();


	elasticity = 0.9f;
}


TableEdge::~TableEdge()
{
}


void TableEdge::draw(sf::RenderWindow &window)
{

}


bool TableEdge::isColliding(const Ball& b) {

	Vector3 radiusAdjust(b.getRadius(), b.getRadius(), 0.0f);
	Vector3 radiusAdjustPosition = b.getPosition() + radiusAdjust;

	Vector3 EC = radiusAdjustPosition - edgeStart;

	//Vector3 segment(0.0f, 346.0f, 0.0f);
	//float dx = 226.0f - 226.0f;
	//float dy = 579.0f - 233.0f;
	//Vector3 normalcoords(-dy, dx, 0.0f);
	//Vector3 normalcoords1(dy, -dx, 0.0f);
	//Vector3 segmentNormal = normalcoords1 - normalcoords;


	Vector3 segmentNormal = normalEdge;
	segmentNormal.Normalise();
	float Dot = Vector3::Dot(segmentNormal, EC);
	//std::cout <<"value of dot product" << Dot << std::endl;
	//std::cout << "value of dot product abs" << abs(Dot) << std::endl;
	//std::cout << cueball->position.x << std::endl;
	//sf::Vector2f SFMLpos(cueball->ball.getPosition());
	//std::cout << SFMLpos.x << std::endl;
	Vector3 edge = edgeLength + edgeStart;
	//if (((radiusAdjustPosition.y <= (edge.y + b.getRadius()) && (abs(Dot) <= b.getRadius()))							//max y
	//	&& ((radiusAdjustPosition.y + b.getRadius()) >= edgeStart.y && (abs(Dot) <= b.getRadius())))					//min y
	//	|| ((((radiusAdjustPosition.x - b.getRadius()) <= edge.x) && (abs(Dot) <= b.getRadius()))						// max x
	//	&& ((radiusAdjustPosition.x + b.getRadius()) >= edgeStart.x && (abs(Dot) <= b.getRadius())))					//min x
	//	){
	//	return true;
	//}
	//else return false;
	
	return (abs(Dot) <= b.getRadius());	
}

bool TableEdge::isColliding2(const Ball& b) 
{
	Vector3 radiusAdjust(b.getRadius(), b.getRadius(), 0.0f);
	Vector3 radiusAdjustPosition = b.getPosition() + radiusAdjust;

	// Calculate perpendicular distance of ball from edge as if the edge extended infinitley

	Vector3  A = edgeStart - radiusAdjustPosition;
	float    parameter = Vector3::Dot(edgeLengthUnit, A);
	Vector3  B = edgeLengthUnit * parameter;
	Vector3  C = A - B;
	float    side = Vector3::Dot(C, edgeNormal);
	
	if (side > 0.0)
	{
		return false;  // Ball is already on the 'wrong' side of the edge
	}

	float perp_distance = C.Length();

	if (perp_distance > b.getRadius())
	{
		return false; // Ball is too far from the edge
	}

	// We can use the value of the paramter above to see of we are within the extent of the edge


	if ((parameter < 0.0f) && (parameter > -edgeLength.Length()))
	{
		return true; // Ball is within limits of edge
	}

	return false;

	// The only possibility now is of a glancing collision with the edge
	float edgeStartDistance = (radiusAdjustPosition - edgeStart).Length();
	float edgeEndDistance   = (radiusAdjustPosition - edgeStart).Length();

	if (edgeStartDistance < b.getRadius())
	{
		return true;
	}

	if (edgeEndDistance < b.getRadius())
	{
		return true;
	}

	std::cout << "No collsion( beyone edge limits)" << std::endl;
	return false;
}

void TableEdge::resolveCollision(Ball &b){

	//Vector3 bcentre(b.getPosition().x + b.getRadius(), b.getPosition().y + b.getRadius(), 0.0f);
	//std::cout << "ball centre x: " << bcentre.x << std::endl;
	//std::cout << "ball centre y: " << bcentre.y << std::endl;
	//Vector3 normal = normalEdge;

	//normal.Normalise();
	//std::cout << "value of normal x: " << normal.x << std::endl;
	//std::cout << "value of normal y: " << normal.y << std::endl;
	//bcentre.Normalise();
	//std::cout << "value of bcentre normalised x: " << bcentre.x << std::endl;
	//std::cout << "value of becntre normalised y: " << bcentre.y << std::endl;
	//float depthpenetration = (b.getRadius() - (Vector3::Dot(normal, bcentre)));
	//std::cout << "value of depthpenetration: " << depthpenetration << std::endl;

	Vector3 vB = b.getVelocity();

	float vN = Vector3::Dot(vB, edgeNormal) ;

	if (vN > 0.0f) 
	{ 
		return; 
	}

	float e = (elasticity + b.getElasticity())*0.5f;

	Vector3 impulse = edgeNormal * (-(1 + e) * vN) ;

	b.setVelocity(vB + impulse);

}

void TableEdge::setEdgeElasticity(const float & e){
	
	elasticity = e;
}















//sf::Vertex leftCushionTopPocket[] =
//{
//	sf::Vertex(sf::Vector2f(216, 218)),
//	sf::Vertex(sf::Vector2f(226, 233))
//
//
//};
//
//sf::Vertex topLeftCushionTopPocket[] =
//{
//	sf::Vertex(sf::Vector2f(230, 206)),
//	sf::Vertex(sf::Vector2f(241, 214))
//
//
//};
//
//sf::Vertex topLeftCushion[] =
//{
//	sf::Vertex(sf::Vector2f(241, 214)),
//	sf::Vertex(sf::Vector2f(590, 214))
//
//
//};
//
//
//
//sf::Vertex topLeftCushionMiddlePocket[] =
//{
//	sf::Vertex(sf::Vector2f(590, 214)),
//	sf::Vertex(sf::Vector2f(599, 205))
//
//
//};
//
//sf::Vertex topRightCushionMiddlePocket[] =
//{
//	sf::Vertex(sf::Vector2f(617, 205)),
//	sf::Vertex(sf::Vector2f(626, 214))
//
//
//};
//
//sf::Vertex topRightCushion[] =
//{
//	sf::Vertex(sf::Vector2f(626, 214)),
//	sf::Vertex(sf::Vector2f(975, 214))
//
//
//};
//
//sf::Vertex topRightCushionTopPocket[] =
//{
//	sf::Vertex(sf::Vector2f(975, 214)),
//	sf::Vertex(sf::Vector2f(986, 206))
//
//
//};
//
//sf::Vertex rightCushionTopPocket[] =
//{
//	sf::Vertex(sf::Vector2f(1001, 216)),
//	sf::Vertex(sf::Vector2f(992, 229))
//
//
//};
//
//sf::Vertex rightCushion[] =
//{
//	sf::Vertex(sf::Vector2f(992, 229)),
//	sf::Vertex(sf::Vector2f(992, 578))
//
//
//};
//
//sf::Vertex rightCushionBottomPocket[] =
//{
//	sf::Vertex(sf::Vector2f(992, 578)),
//	sf::Vertex(sf::Vector2f(1001, 592))
//
//
//};
//
//sf::Vertex bottomRightCushionPocket[] =
//{
//	sf::Vertex(sf::Vector2f(987, 603)),
//	sf::Vertex(sf::Vector2f(974, 596))
//
//
//};
//
//sf::Vertex bottomRightCushion[] =
//{
//	sf::Vertex(sf::Vector2f(974, 596)),
//	sf::Vertex(sf::Vector2f(625, 596))
//
//
//};
//
//sf::Vertex bottomRightCushionMiddlePocket[] =
//{
//	sf::Vertex(sf::Vector2f(625, 596)),
//	sf::Vertex(sf::Vector2f(618, 605))
//
//
//};
//
//
//sf::Vertex bottomLeftCushionMiddlePocket[] =
//{
//	sf::Vertex(sf::Vector2f(598, 605)),
//	sf::Vertex(sf::Vector2f(590, 596))
//
//
//};
//
//sf::Vertex bottomLeftCushion[] =
//{
//	sf::Vertex(sf::Vector2f(590, 596)),
//	sf::Vertex(sf::Vector2f(241, 596))
//
//
//};
//
//sf::Vertex bottomLeftCushionPocket[] =
//{
//	sf::Vertex(sf::Vector2f(241, 596)),
//	sf::Vertex(sf::Vector2f(230, 605))
//
//
//};
//
//sf::Vertex leftCushionBottomPocket[] =
//{
//	sf::Vertex(sf::Vector2f(215, 592)),
//	sf::Vertex(sf::Vector2f(226, 579))
//
//
//};
//
//sf::Vertex leftCushion[] =
//{
//	sf::Vertex(sf::Vector2f(226, 233)),
//	sf::Vertex(sf::Vector2f(226, 579))
//
//
//};
