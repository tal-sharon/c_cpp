
// don't change those includes
#include "RSUser.h"

// implement your cpp code here
RSUser::RSUser (const std::string &name, rank_map &ranks,
                std::shared_ptr<RecommenderSystem>& rs)
                : _name(name), _rs(rs)
{
  rank_map watched (0, sp_movie_hash, sp_movie_equal);
  for (const auto& movie : ranks)
  {
    if (movie.second > 0)
    {
      watched[movie.first] = movie.second;
    }
  }
  _watched = watched;
}

const std::string &RSUser::get_name () const
{
  return _name;
}

void
RSUser::add_movie_to_rs (const std::string &name, int year,
                         const std::vector<double> &features, double rate)
{
  sp_movie movie = _rs->add_movie(name, year, features);
  _watched[movie] = rate;

}

const rank_map &RSUser::get_ranks () const
{
  return _watched;
}

sp_movie RSUser::get_recommendation_by_content () const
{
  return sp_movie (_rs->recommend_by_content (*this));
}

sp_movie RSUser::get_recommendation_by_cf (int k) const
{
  return sp_movie (_rs->recommend_by_cf (*this, k));
}

double
RSUser::get_prediction_score_for_movie (const std::string &name,
                                        int year, int k) const
{
  Movie m(name, year);
  sp_movie p = std::make_shared<Movie>(m);
  return _rs->predict_movie_score(*this, p, k);
}

std::ostream &operator<< (std::ostream &os, const RSUser &user)
{
  os << "name: " << user.get_name() << std::endl;
  os << *(user._rs) << std::endl;
  return os;
}

