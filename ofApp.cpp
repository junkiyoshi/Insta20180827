#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofTrueTypeFont font;
	font.loadFont("fonts/Kazesawa-bold.ttf", 350, true, true, true);
	string word = "EX";
	int sample_count = 200;
	this->number_of_targets = sample_count * word.size();

	ofPoint word_size(font.stringWidth(word), font.stringHeight(word));
	vector<ofPath> word_path = font.getStringAsPoints(word, true, false);
	for (int word_index = 0; word_index < word.size(); word_index++) {

		vector<ofPolyline> outline = word_path[word_index].getOutline();
		int tmp_sample_count = sample_count / outline.size();
		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			outline[outline_index] = outline[outline_index].getResampledByCount(tmp_sample_count);
			vector<glm::vec3> vertices = outline[outline_index].getVertices();
			for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

				glm::vec4 target = glm::vec4(vertices[vertices_index].x, vertices[vertices_index].y, vertices[vertices_index].z, 0.0);
				target += glm::vec4(word_size.x * -0.5 + ofGetWidth() * 0.5, word_size.y * 0.5 + ofGetHeight() * 0.5, 0.0, 0.0);
				this->targets.push_back(target);

				glm::vec4 color = glm::vec4(1.0, 1.0, 1.0, 0.0);
				this->sizes.push_back(1.0);
			}
		}
	}

	while (this->targets.size() < sample_count * word.size()) {

		this->targets.push_back(glm::vec4(0.0, 0.0, 0.0, 0.0));
		this->sizes.push_back(0.0);
	}

	cout << this->targets.size() << endl;

	this->shader.load("shader/shader.vert", "shader/shader.frag");
}
//--------------------------------------------------------------
void ofApp::update() {
	
	for (int i = 0; i < this->targets.size(); i++) {

		if (this->targets[i] == glm::vec4(0.0, 0.0, 0.0, 0.0)) { continue; }

		//this->sizes[i] = ofNoise(this->targets[i].x * 0.05, this->targets[i].y * 0.05, ofGetFrameNum() * 0.05);
		this->sizes[i] = ofRandom(1.f);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	vector<glm::vec4> points;
	for (int i = 0; i < this->number_of_targets; i++) {

		glm::vec4 p = glm::vec4(this->targets[i].x, this->targets[i].y, 0, 0);
		points.push_back(p);
	}

	this->shader.begin();
	this->shader.setUniform1f("time", ofGetElapsedTimef());
	this->shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	this->shader.setUniform4fv("targets", &points[0].x, this->number_of_targets);
	this->shader.setUniform1fv("sizes", &this->sizes[0], this->number_of_targets);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());

	this->shader.end();
}

//--------------------------------------------------------------
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(720, 720);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}