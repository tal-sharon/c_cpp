#include <fstream>
#include <sstream>
#include "RSUsersLoader.h"


//////////////////////////////////////////////////////////////////////////////

// helper functions declarations

std::stringstream get_users_stream (const std::string &users_file_path);

void
get_movies_from_file (std::vector<std::string> &movies_vec,
                      std::map<std::string, int> &movies_map,
                      std::stringstream &users_file);

void
get_movie_rank (std::shared_ptr<RecommenderSystem> &s_rs,
                std::map<std::string, int> &movies_map,
                const std::vector<std::string> &movies_vec,
                int i, std::string &buf, std::string &user_name,
                rank_map &ranks);

void
process_users_info (std::shared_ptr<RecommenderSystem> &s_rs,
                    std::map<std::string,
                     int> &movies_map, std::stringstream &users_file,
                    const std::vector<std::string> &movies_vec,
                    std::vector<RSUser> &users);

void
parse_movies (std::vector<std::string> &movies_vec, std::map<std::string,
              int> &movies_map, std::stringstream &ss,
              std::string &movie_name, int movie_year);



//////////////////////////////////////////////////////////////////////////

// code implementation

std::vector<RSUser>
RSUsersLoader::create_users_from_file (const std::string &users_file_path,
                                       std::unique_ptr<RecommenderSystem> rs)
                                       noexcept (false)
{
  std::shared_ptr<RecommenderSystem> s_rs = std::move (rs);
  std::map<std::string, int> movies_map;
  std::stringstream users_file = get_users_stream (users_file_path);
  std::vector<std::string> movies_vec;
  get_movies_from_file (movies_vec, movies_map, users_file);
  std::vector<RSUser> users;
  process_users_info (s_rs, movies_map, users_file, movies_vec, users);
  return users;
}

void
process_users_info (std::shared_ptr<RecommenderSystem> &s_rs,
                    std::map<std::string,
                    int> &movies_map, std::stringstream &users_file,
                    const std::vector<std::string> &movies_vec,
                    std::vector<RSUser> &users)
{
  std::string line;
  while (std::getline(users_file, line))
  {
    std::string user_name;
    std::stringstream user_info(line);
    rank_map ranks (0, sp_movie_hash, sp_movie_equal);
    int i = USER_NAME_INDEX;
    std::string buf;
    while (std::getline(user_info, buf, SPACE_DELIMITER))
    {
      get_movie_rank (s_rs, movies_map, movies_vec, i, buf, user_name, ranks);
      i++;
    }
    RSUser user(user_name, ranks, s_rs);
    users.push_back (user);
  }
}

void
get_movie_rank (std::shared_ptr<RecommenderSystem> &s_rs,
                std::map<std::string, int> &movies_map,
                const std::vector<std::string> &movies_vec,
                int i, std::string &buf, std::string &user_name,
                rank_map &ranks)
{
  if (i == USER_NAME_INDEX)
  {
    user_name = buf;
  }
  else
  {
    double rank = 0;
    if (buf != "NA")
    {
      std::stringstream rank_str(buf);
      rank_str >> rank;
      if ( rank < MIN_RANK || rank > MAX_RANK)
      {
        throw std::runtime_error (ERR_MSG_INVALID_RANK);
      }
    }
    std::string m_name = movies_vec[i];
    int m_year = movies_map[m_name];
    sp_movie new_movie = s_rs->get_movie (m_name, m_year);
    ranks.insert(std::make_pair (new_movie, rank));
  }
}

void
get_movies_from_file (std::vector<std::string> &movies_vec,
                      std::map<std::string, int> &movies_map,
                      std::stringstream &users_file)
{
  std::string movies_line;
  std::getline(users_file, movies_line);
  std::stringstream ss(movies_line);
  std::string movie_name;
  int movie_year = 0;
  parse_movies (movies_vec, movies_map, ss, movie_name, movie_year);
}

void
parse_movies (std::vector<std::string> &movies_vec, std::map<std::string,
              int> &movies_map, std::stringstream &ss,
              std::string &movie_name, int movie_year)
{
  while (ss >> movie_name)
  {
    std::string buffer;
    std::stringstream cur(movie_name);
    std::getline(cur, buffer, NAME_YEAR_DELIMITER);
    cur >> movie_year;
    if (movies_map.find(buffer) == movies_map.end())
    {
      movies_vec.push_back(buffer);
      movies_map[buffer] = movie_year;
    }
    else
    {
      throw std::invalid_argument (ERR_MSG_DUPLICATED_KEY);
    }
  }
}

std::stringstream get_users_stream (const std::string &users_file_path)
{
  std::ifstream is(users_file_path);
  if (!is)
  {
    throw std::runtime_error(ERR_MSG_OPEN_FILE);
  }
  if (is)
  {
    is.seekg (0, std::ifstream::end);
    long length = is.tellg();
    is.seekg (0, std::ifstream::beg);
    char *buffer = new char [length];
    is.read (buffer, length);
    if (is)
    {
      std::stringstream users_file((std::string(buffer)));
      delete[] buffer;
      return users_file;
    }
    else
    {
      delete[] buffer;
      throw std::runtime_error(ERR_MSG_READ_FILE);
    }
  }

  std::stringstream users_file;
  return users_file;
}

