class Malla {
private:
  bool* matrix;
  int rows;
  int cols;

public:
  // constructor
  Grid(unsigned xin, unsigned yin){
  if (xin>0 && yin >0){
	this->rows = xin;
	this->cols = yin;

	this->matrix =  new bool[rows*cols];
	std::fill_n(matrix, rows * cols, false);
  }
  else {
	throw std::invalid_argument("Size of grid, must be rows>0 and cols>0");
  }
};
  // square constructor
  Grid(unsigned vin){
  if(vin>0){
	this->rows = vin;
	this->cols = vin;
	this->matrix =  new bool[rows*cols];
	std::fill_n(matrix, rows * cols, false);
	}
  else {
	throw std::invalid_argument("Size of grid, must be vin>0");
  }
};
  // default constructor
  Malla(){
	this->rows = 10;
	this->cols = 10;
	this->matrix =  new bool[rows*cols];
	std::fill_n(matrix, rows * cols, false);
  };
  // constructor given a string with array
  Malla(string grid){
   vector<vector<bool>> matrix;
   matrix = validate_matrix_str(grid);
   load_grid(matrix); 
  };
  // load grid from file
  Malla(std::filesystem::path path){
  if (std::filesystem::is_regular_file(path)){
	  std::ifstream file(path, std::ios::in | std::ios::binary);
	  if (!file.is_open()){
			throw std::invalid_argument("File cannot be opened");
		  };
	  // Read contents
	  std::string grid{
		std::istreambuf_iterator<char>(file), 
		std::istreambuf_iterator<char>()
	  };
	  // Close the file
	  file.close();
	  vector<vector<bool>> matrix;
	  matrix = validate_matrix_str(grid);
	  load_grid(matrix);
  } else {
	throw std::invalid_argument("File path must be a filename path and be readabale");
  }
  };
  // create the next grid
  Malla(Malla &old_grid){
  this->rows = old_grid.getRows();
  this->cols = old_grid.getCols();

  this->matrix =  new bool[rows*cols];

  std::fill_n(matrix, rows * cols, false);
 
   for (int i = 0; i < rows; i++)  {
	 for ( int j = 0; j < cols; j++) {
	   this->matrix[i * cols + j] = old_grid.check_next(i,j);
	 }
   }
};
  // check next
  bool check_next(unsigned xin, unsigned yin){
  if (xin<rows && yin<cols) {
	  unsigned check = 0;
	  unsigned up = 0;
	  up = rows-1;
	  if (xin > 0){
		up = xin - 1;
	  } 
	 
	  unsigned down = xin + 1;
	  if (down==rows) {
		down=0;
	  }

	  unsigned left = 0;
	  left = cols -1;
	  if (yin>0){
		left = yin - 1;
	  } 

	  unsigned right = yin + 1;
	  if (right==cols) {
		right=0;
	  }

	  bool cell =  getValue(xin,yin);
	  int up_val = getValue(up,yin);
	  int down_val = getValue(down,yin);
	  int left_val = getValue(xin,left);
	  int right_val = getValue(xin,right);
	  check = up_val + down_val + left_val + right_val;
	  if (cell==false && check==3) {
		return true;
	  } else if (cell==true && (check==2 || check==3)) {
		return true;
	  } else {
		return false;
	  }
  } else {
	throw std::invalid_argument("Índices fuera de rango");
  }
  };
  // load grid
  void load_grid(vector<vector<bool>> &matrix){
    this->rows = matrix.size();
	this->cols = matrix[0].size();
	this->matrix =  new bool[rows*cols];

	for (int i=0;i<rows; i++) {
	for (int j=0;j<cols; j++) {
	  this->matrix[i*cols+j]=matrix[i][j];
	}
	}
  };

  void gen_random(int seed){
	  srand(seed);
	  auto gen = std::bind(std::uniform_int_distribution<>(0,1),std::default_random_engine());

		for (int i=0;i<getRows(); i++) {
		for (int j=0;j<getCols(); j++) {
		  bool random = gen();
		  this->matrix[i*cols + j] = random;
		  }
		}
  };

  // load grid from string
  // generate next grid
  vector<vector<bool>> next(){
	  

  };
  // show grid
  friend std::ostream& operator<<(std::ostream &os, Grid &grid){
   for (int i = 0; i < grid.getRows(); i++)  {
	 for ( int j = 0; j < grid.getCols(); j++) {
	   int value = grid.getValue(i,j);
	   string symbol = value?"X":"O";
	   os << symbol << " ";
	 }
	 os<<endl;
   } 

 return os;

  };

  int getRows(){
	  return this->rows;
  };

  int getCols(){
	  return this->cols;
  };

  bool getValue(int i, int j){
  if (i<getRows() && j<getCols()) {
	return this->matrix[i*cols+j];
  } else {
	throw std::invalid_argument("Indexes must be low than number of rows and cols");
  }
  };

  bool operator==(const Grid& other) const{
	if (rows != other.rows || cols != other.cols){
	  return false;
	}

	for (int i=0;i<rows*cols; i++) {
	  int check = other.matrix[i];
	  if(matrix[i]!=check) {
		return false;
	  }
	}
	return true;
  };

  bool has_life() {
	for (int i=0;i<getRows(); i++) {
	for (int j=0;j<getCols(); j++) {
	  bool val = getValue(i,j);
	  if (val) {
		return true;
	  }
	  }
	}
	return false;
  };

  int alive(){
	int alive = 0;
	for (int i=0;i<getRows(); i++) {
	for (int j=0;j<getCols(); j++) {
	  bool val = getValue(i,j);
	  if (val) {
		alive++;
	  }
	  }
	}
	return alive;
  };
  int dead(){
	int dead = 0;
	for (int i=0;i<getRows(); i++) {
	for (int j=0;j<getCols(); j++) {
	  bool val = getValue(i,j);
	  if (!val) {
		dead++;
	  }
	  }
	}
	return dead;
  };

  ~Malla(){
	  delete[] matrix;
  };
};
