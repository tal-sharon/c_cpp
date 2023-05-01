
#ifndef SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
#define SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
#define NEGATIVE_RATE -1
#include "RSUser.h"
#include <map>

class RSUser;

//////////////////////////////////////////////////////////////////////////////

// structs and typedefs

/**
 * comparator struct for map<sp_movie, vector<double>>
 */
struct cmp_movie
{
    bool operator()(const sp_movie& lhs, const sp_movie& rhs) const
    {
      return *lhs < *rhs;
    }
};

typedef std::vector<double> features_vec;

typedef std::vector<std::pair<sp_movie, double>> rates_vec;

typedef std::pair<std::map<sp_movie, features_vec>,
    std::map<sp_movie, features_vec>> movie_maps_pair;

//////////////////////////////////////////////////////////////////////////////


class RecommenderSystem
{
 private:
  std::map<sp_movie, features_vec, cmp_movie> _movies;
  unsigned int _feat_vec_len;

  /**
   * compare function for descending sort a vector of pair<sp_movie,
   * double> by the double value representing the rank of the movie
   * @param lhs left-hand-side pair<sp_movie, double>
   * @param rhs left-hand-side pair<sp_movie, double>
   * @return true if lhs.second is bigger than rhs.second
   */
  static bool cmp_movie_by_similarity(const std::pair<sp_movie, double>& lhs,
                                   const std::pair<sp_movie, double>& rhs)
  {
    return lhs.second > rhs.second;
  }

  /**
   * calculates and get the ranks average of a given user
   * @param user a given RSUser
   * @return the user's ranks average
   */
  double get_user_ranks_avg (const RSUser &user) const;

  /**
   * fills preferences vector of a user and calculates it's norm
   * @param user a given RSUser
   * @param ranks_avg double the ranks average of the user
   * @param pref_vec an empty vector<double> preferences vector
   * @return double: the norm of the filled preferences vector
   */
  double
  fill_pref_vec (const RSUser &user, double ranks_avg, features_vec &pref_vec);

  /**
   * calculates the norm of a given features vector
   * @param vec vector<double>: a features vector of a movie
   * @return double: the norm of the given vector
   */
  double calc_norm (const features_vec & vec);

  /**
   *
   * @param user a given RSUser
   * @param vec vector<double>: a features vector of a movie
   * @param norm double: the norm of the given features vector
   * @return sp_movie: the movie which it's features vector is most similar to
   * the given vector
   */
  sp_movie
  get_similar_by_content (const RSUser &user, const features_vec &vec,
                          double norm);

  /**
   * separates all the movies of a RecommenderSystem to a users watched and
   * not watched maps
   * @param user a given RSUser
   * @return a pair of two map<sp_movie, features_vec>,
   * first is not_watched by the user, second is watched by the user.
   */
  movie_maps_pair
  make_movie_maps (const RSUser &user);

  /**
   * predicts the rank of a given movie for a specific given user.
   * @param user a given RSUser
   * @param k int: the number of movies to compare
   *                from the user's watched movies
   * @param watched map<sp_movie, features_vec>:
   *                    all the movies the user watched
   * @param not_watched_movie pair<const sp_movie, features_vec>:
   *                            the movie to predict it's rank
   * @return double: the predicted rank
   */
  double
  predict_rate (const RSUser &user, int k, std::map<sp_movie,
                features_vec> &watched, const std::pair<const sp_movie,
                features_vec> &not_watched_movie);

  /**
   *
   * @param watched map<sp_movie, features_vec>:
   *                    all the movies the user watched
   * @param not_watched_movie pair<const sp_movie, features_vec>:
   *                            the movie to predict it's rank
   * @param norm double: the norm of the features vector
   *                        of the not_watched_movie
   * @param rates vector<pair<sp_movie, double>>:
   *                vector of similarity values of watched movies with the
   *                not_watched movie
   */
  void
  get_similarities (std::map<sp_movie, features_vec> &watched,
                    const std::pair<const sp_movie,
                    features_vec> &not_watched_movie, double norm,
                    rates_vec &rates) const;

  /**
   * calculates the predicted rank of a not watched movie of a given user
   * @param user a given RSUser
   * @param k int: the number of movies to compare
   *                from the user's watched movies
   * @param rates vector<pair<sp_movie, double>>:
   *                vector of similarity values of watched movies with the
   *                not_watched movie
   * @return double: the predicted rank
   */
  double
  calc_prediction (const RSUser &user, int k, const
  rates_vec &rates) const;

 public:
  //explicit RecommenderSystem()
    /**
     * adds a new movie to the system
     * @param name name of movie
     * @param year year it was made
     * @param features features for movie
     * @return shared pointer for movie in system
     */
	sp_movie add_movie(const std::string& name,int year,
                       const std::vector<double>& features);

    /**
     * a function that calculates the movie with highest score based on movie
     * features
     * @param ranks user ranking to use for algorithm
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_content(const RSUser& user);

    /**
     * a function that calculates the movie with highest predicted score based
     * on ranking of other movies
     * @param ranks user ranking to use for algorithm
     * @param k
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_cf(const RSUser& user, int k);


    /**
     * Predict a user rating for a movie given argument using item cf procedure
     * with k most similar movies.
     * @param user_rankings: ranking to use
     * @param movie: movie to predict
     * @param k:
     * @return score based on algorithm as described in pdf
     */
	double predict_movie_score(const RSUser &user, const sp_movie &movie,
												  int k);

	/**
	 * gets a shared pointer to movie in system
	 * @param name name of movie
	 * @param year year movie was made
	 * @return shared pointer to movie in system
	 */
	sp_movie get_movie(const std::string &name, int year) const;

	friend std::ostream& operator<<(std::ostream& os, const RecommenderSystem&
    rs);

};


#endif //SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H