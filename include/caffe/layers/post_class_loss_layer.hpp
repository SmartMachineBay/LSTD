#ifndef CAFFE_POST_CLASS_LOSS_LAYER_HPP_
#define CAFFE_POST_CLASS_LOSS_LAYER_HPP_

#include <map>
#include <utility>
#include <vector>

#include "caffe/blob.hpp"
#include "caffe/layer.hpp"
#include "caffe/proto/caffe.pb.h"
#include "caffe/util/bbox_util.hpp"

#include "caffe/layers/loss_layer.hpp"

namespace caffe {

/**
 * @brief Perform MultiBox operations. Including the following:
 *
 *  - decode the predictions.
 *  - perform matching between priors/predictions and ground truth.
 *  - use matched boxes and confidences to compute loss.
 *
 */
template <typename Dtype>
class PostClassLossLayer : public LossLayer<Dtype> {
 public:
  explicit PostClassLossLayer(const LayerParameter& param)
      : LossLayer<Dtype>(param) {}
  virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);

  virtual inline const char* type() const { return "PostClassLoss"; }
  // bottom[0] stores the location predictions.
  // bottom[1] stores the confidence predictions.
  // bottom[2] stores the prior bounding boxes.
  // bottom[3] stores the ground truth bounding boxes.
  virtual inline int ExactNumBottomBlobs() const { return 3; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

 protected:
  virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Backward_cpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);

  // The internal confidence loss layer.
  shared_ptr<Layer<Dtype> > conf_loss_layer_;
  ConfLossType conf_loss_type_;
  // bottom vector holder used in Forward function.
  vector<Blob<Dtype>*> conf_bottom_vec_;
  // top vector holder used in Forward function.
  vector<Blob<Dtype>*> conf_top_vec_;
  // blob which stores the confidence prediction.
  Blob<Dtype> conf_pred_;
  // blob which stores the corresponding ground truth label.
  Blob<Dtype> conf_gt_;
  // confidence loss.
  Blob<Dtype> conf_loss_;

  MultiBoxLossParameter multibox_loss_param_;
  int num_classes_;
  bool share_location_;
  MatchType match_type_;
  float overlap_threshold_;
  int background_label_id_;
  bool use_difficult_gt_;
  bool do_neg_mining_;
  float neg_pos_ratio_;
  float neg_overlap_;
  bool encode_variance_in_target_;
  bool map_object_to_agnostic_;
  bool ignore_cross_boundary_bbox_;
  bool bp_inside_;
  MiningType mining_type_;

  int loc_classes_;
  int num_gt_;
  int num_;
  int num_priors_;

  int num_matches_;
  int num_conf_;
  vector<map<int, vector<int> > > all_match_indices_;
  vector<vector<int> > all_neg_indices_;

  // How to normalize the loss.
  LossParameter_NormalizationMode normalization_;
};

}  // namespace caffe

#endif  // CAFFE_POST_CLASS_LOSS_LAYER_HPP_
