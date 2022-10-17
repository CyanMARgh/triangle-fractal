#include "utils.h"
#include <cstdio>
#include <SFML/Graphics.hpp>
#include <random>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <vector>

struct Polyline : std::vector<vec2> {
	template<typename... Arg_Types>
	Polyline(Arg_Types... args) : std::vector<vec2>(args...) {}
	float L;
};
struct Polyline_Set : std::vector<Polyline> {
	float L;
};
void update_length(Polyline& polyline) {
	float L = 0.f;
	for(u32 i = 1, n = polyline.size(); i < n; i++) {
		L += len(polyline[i] - polyline[i-1]);
	}
	polyline.L = L;
}
void update_length(Polyline_Set& polyline_set) {
	float L = 0.f;
	for(auto& polyline : polyline_set) {
		update_length(polyline);
		L += polyline.L;
	}
	polyline_set.L = L;
	printf("L = %f\n", L);
}
vec2 lerp(const Polyline& polyline, float t) {
	t *= polyline.L;
	float ta = 0.f;
	for(u32 i = 1, n = polyline.size(); i < n; i++) {
		vec2 A = polyline[i - 1], B = polyline[i];
		float dt = len(B - A);
		if(ta + dt > t) return lerp(A, B, (t - ta) / dt);
		ta += dt;
	}
	return polyline.back();
}
vec2 lerp(const Polyline_Set& polyline_set, float t) {
	t *= polyline_set.L;
	float t_ = 0.f;
	for(const auto& polyline : polyline_set) {
		float dt = polyline.L;
		if(t_ + dt > t) {
			return lerp(polyline, (t - t_) / dt);
		}
		t_ += dt;
	}
	return polyline_set.back().back();
}

// const u32 MAX_POINTS = 5;
std::string string_time() {
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::ostringstream oss;
	oss << std::put_time(&tm, "(%d-%m-%Y %H-%M-%S)");
	return oss.str();
}

struct Node {
	vec2 point = {0.f, 0.f};
	Node *left = nullptr, *right = nullptr;
};
struct Tree {
	u32 X, Y;
	std::vector<Node*> nodes;
};

Node* add_point(Node* node, vec2 p) {
	// printf("[%f, %f]", p.x, p.y);
	if(!node) {
		// printf("(0)");
		node = new Node;
		node->point = p;
	} else {
		if(p.x > p.y) {
			// printf("(1)");
			node->right = add_point(node->right, {1.f - p.x - p.y, p.x - p.y});
		} else {
			// printf("(2)");
			node->left = add_point(node->left, {p.y - p.x, 1.f - p.x - p.y});
		}
	}
	return node;
}
void free(Node* node) {
	if(node) {
		free(node->left);
		free(node->right);
	}
	delete node;
}
Tree make_tree(u32 X, u32 Y) {
	Tree tree;
	tree.X = X, tree.Y = Y;
	tree.nodes = std::vector<Node*>(X * Y * 2, nullptr);
	return tree;
}
void free(Tree* tree) {
	for(auto n : tree->nodes) {
		free(n);
	}
}
void add_point(Tree* tree, vec2 p) {
	u32 X = tree->X, Y = tree->Y, I = 0;
	int ix = (int)p.x, iy = (int)p.y;	
	ix = ix < 0 ? 0 : ix >= X ? X - 1 : ix; 
	iy = iy < 0 ? 0 : iy >= Y ? Y - 1 : iy; 
	p -= vec2{(float)ix, (float)iy};
	if(p.x + p.y > 1.f) p = 1.f - p, I = 1;
	u32 id = (X * iy + ix) * 2 + I;

	tree->nodes[id] = add_point(tree->nodes[id], p);
}
long hash64(u64 h) {
	h ^= h >> 33 | h << 31;
	h *= 0xff51afd7ed558ccdL;
	h ^= h >> 33 | h << 31;
	h *= 0xc4ceb9fe1a85ec53L;
	h ^= h >> 33 | h << 31;
	return h;
}
const Node* find(const Node* node, vec2 p) {
	if (p.x > p.y) {
		// printf("(1)\n");
		if(node->right) return find(node->right, {1.f - p.x - p.y, p.x - p.y});
	} else {
		// printf("(2)\n");
		if(node->left) return find(node->left, {p.y - p.x, 1.f - p.x - p.y});
	}
	return node;
}
u32 get_hash(const Tree* tree, vec2 p) {
	u32 X = tree->X, Y = tree->Y, I = 0;
	int ix = (int)p.x, iy = (int)p.y;	
	ix = ix < 0 ? 0 : ix >= X ? X - 1 : ix; 
	iy = iy < 0 ? 0 : iy >= Y ? Y - 1 : iy; 
	p -= vec2{(float)ix, (float)iy};
	if(p.x + p.y > 1.f) p = 1.f - p, I = 1;
	u32 id = (X * iy + ix) * 2 + I;
	
	const Node* result = nullptr;
	if(tree->nodes[id]) result = find(tree->nodes[id], p);
	
	return (u32)hash64((u64)result); 
}
void draw(sf::RenderWindow& rw, const Polyline& polyline) {
	u32 n = polyline.size();
	sf::Vertex *vlines = new sf::Vertex[n];
	for(u32 i = 0; i < n; i++) {
		vlines[i] = to_sfml(polyline[i]);
	}
	rw.draw(vlines, n, sf::LineStrip);
}
void draw(sf::RenderWindow& rw, const Polyline_Set& polyline_set) {
	for(const auto& polyline : polyline_set) {
		draw(rw, polyline);
	}
}

const u32 PALETTE_SIZE = 4;
const u32 palette[PALETTE_SIZE] = {
	// 0xFFB2B1,
	// 0xFFC4AA,
	// 0xFFDAD2,
	// 0xFFF1EE

	// 0xD5FA2A,
	// 0xDAE02F,
	// 0xFAEF96,
	// 0xFFA08F,
	// 0xEDBC26

	// 0xDBFFFE,
	// 0x2AFAF3,
	// 0x2FE0DA,
	// 0x6FFAF6,
	// 0x26EDE7

	// 0x845EC2,
	// 0x4FFBDF,
	// 0x00C2A8,
	// 0x008B74

	// 0x845EC2,
	// 0xD65DB1,
	// 0xFF6F91,
	// 0xFF9671,
	// 0xFFC75F,
	// 0xF9F871

	0xA7D2CB,
	0xF2D388,
	0xC98474,
	0x874C62
};
u32 rgb_to_abgr(u32 c) {
	return 0xFF000000 | (c & 0xFF0000) >> 16 | (c & 0xFF00) | (c & 0xFF) << 16;
}
sf::Image make_image(const Tree* tree, u32 size, u32 X, u32 Y) {
	sf::Image result;
	result.create(size * X, size * Y);
	u32* pixels = (u32*)result.getPixelsPtr();
	vec2 m = {Y / (size * Y - 1.f), X / (size * X - 1.f)};

	for(u32 y = 0, i = 0; y < size * Y; y++) {
		for(u32 x = 0; x < size * X; x++, i++) {
			pixels[i] = rgb_to_abgr(palette[get_hash(tree, vec2{(float)x, (float)y} * m) % PALETTE_SIZE]);
		}
	}
	return result;
}

int main() {
	const uint32_t win_size = 120, X = 10, Y = 7;
	//Polyline_Set polyline_set;
	Polyline polyline;

	sf::CircleShape cursor;
	sf::RenderWindow window(sf::VideoMode(win_size * X, win_size * Y), "", sf::Style::Close);

	bool pressed = false;
	while(window.isOpen()) {
		vec2 mpos = to_cm((sf::Vector2f)sf::Mouse::getPosition(window));
		for(sf::Event e; window.pollEvent(e);) {
			if(e.type == sf::Event::MouseButtonPressed) {
				//polyline_set.push_back({});
				pressed = true;
			} else if(e.type == sf::Event::MouseButtonReleased) {
				if(!polyline.size() || (polyline.back() != mpos)) {
					polyline.push_back(mpos);
				}
				pressed = false;
			} else if(e.type == sf::Event::Closed) {
				window.close();
			} else if(e.type == sf::Event::KeyPressed) {
				if(e.key.code == sf::Keyboard::Enter) window.close();
			}
		}
		// if(pressed) {
		// 	auto& polyline = polyline_set.back();
		// 	if(!polyline.size() || (polyline.back() != mpos && !(I++ % 300))) {
		// 		polyline.push_back(mpos);
		// 	}
		// }
		window.clear();
		draw(window, polyline);
		window.display();
	}

	// update_length(polyline_set);
	update_length(polyline);
	auto pow_rand = [] () -> float {
		float r = randf() * 2.f - 1.f;
		r = pow(fabs(r), .5f) * (r > 0 ? 1.f : -1.f);
		return r;
	};
	std::random_device rd{};
	std::mt19937 gen{rd()};
	std::normal_distribution<> d{0.f, 1.f};

	Tree tree = make_tree(X, Y);
	for(u32 i = 0, M = 10000; i < M; i++) {
		float t = randf();
		// vec2 dp = vec2{pow_rand(), pow_rand()} * .1f - .05f;
		vec2 dp = vec2{(float)d(gen), (float)d(gen)};
		dp *= .15;
		vec2 p = lerp(polyline, t) / win_size;
		add_point(&tree, p + dp);
	}
	printf("ok\n");
	sf::Image image = make_image(&tree, 120, X, Y);
	image.saveToFile("output/" + string_time() + ".png");
	free(&tree);
	return 0;
}