package io.agora.liveshop.widgets;

import android.app.Activity;
import android.graphics.drawable.GradientDrawable;
import android.os.Handler;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;

import java.util.List;

import io.agora.liveshop.R;
import io.agora.liveshop.data.Quiz;
import io.agora.liveshop.data.QuizResult;

public class QuizDialog implements View.OnClickListener {
    private static final int MIN_OPTION_PROGRESS_WIDTH = 100;

    private Activity mActivity;
    private LayoutInflater mInflater;
    private Handler mHandler;
    private View mContainer;
    private ProgressBar mProgressBar;
    private TextView mQuizContent;
    private TextView mQuizCount;
    private LinearLayout mOptionLayout;
    private int mSelectedIndex = -1;
    private Quiz mQuiz;

    private Runnable mCountRunnable = new Runnable() {
        @Override
        public void run() {
            int progress = mProgressBar.getProgress();
            if (progress > 0) {
                mProgressBar.incrementProgressBy(-1);
                mQuizCount.setText(numToString(mProgressBar.getProgress()));
                mHandler.postDelayed(mCountRunnable, 1000);
            } else {
                showResult();
            }
        }
    };

    private void showResult() {
        QuizResult result = mQuiz.getResult();
        calculateProgress(result.getSpread(), result.getTotal(), result.getResult());
    }

    private void calculateProgress(int[] distribution, int total, int answer) {
        for (int i = 0; i < mOptionLayout.getChildCount(); i++) {
            View view = mOptionLayout.getChildAt(i);
            view.setSelected(false);

            // calculate the length of the progress based on the ratio
            // of population and the length of the entire option
            View optionRatioView = view.findViewById(R.id.quiz_option_ratio);
            optionRatioView.setVisibility(View.VISIBLE);
            int width = view.getMeasuredWidth() * distribution[i] / total;
            if (width < MIN_OPTION_PROGRESS_WIDTH) {
                // in case that the progress is smaller than the corner
                width = MIN_OPTION_PROGRESS_WIDTH;
            }
            ViewGroup.LayoutParams params = optionRatioView.getLayoutParams();
            params.width = width;
            optionRatioView.setLayoutParams(params);

            GradientDrawable d = (GradientDrawable) optionRatioView.getBackground();
            if (i == answer) {
                d.setColor(mActivity.getResources().getColor(R.color.green_light));
            } else if (i == mSelectedIndex) {
                d.setColor(mActivity.getResources().getColor(R.color.red_light));
            } else {
                d.setColor(mActivity.getResources().getColor(R.color.lightGray));
            }

            TextView chosenText = view.findViewById(R.id.quiz_option_chosen);
            chosenText.setVisibility(View.VISIBLE);
            chosenText.setText(String.valueOf(distribution[i]));
        }
    }

    private String numToString(int num) {
        if (num >= 10) {
            return num + "";
        } else if (num > 0) {
            return "0" + num;
        } else {
            return "00";
        }
    }

    public QuizDialog(Activity activity, View container) {
        mActivity = activity;
        mInflater = LayoutInflater.from(mActivity);
        mHandler = new Handler(mActivity.getMainLooper());
        mContainer = container;
    }

    private void initUI(Quiz quiz) {
        if (quiz == null) {
            return;
        }

        mContainer.setVisibility(View.VISIBLE);
        mProgressBar = mContainer.findViewById(R.id.quiz_progress);
        mQuizContent = mContainer.findViewById(R.id.quiz_content);
        mQuizCount = mContainer.findViewById(R.id.quiz_count_text);

        mProgressBar = mContainer.findViewById(R.id.quiz_progress);
        mProgressBar.setIndeterminate(false);
        mProgressBar.setMax(quiz.getTimeout());
        mProgressBar.setProgress(quiz.getTimeout());
        mQuizCount.setText(numToString(mProgressBar.getMax()));

        mQuizContent.setText(quiz.getContent());

        mOptionLayout = mContainer.findViewById(R.id.quiz_options);
        if (mOptionLayout.getChildCount() > 0) {
            mOptionLayout.removeAllViews();
        }

        List<String> options = quiz.getOptions();
        for (int i = 0; i < options.size(); i++) {
            View view = mInflater.inflate(R.layout.quiz_option_item_layout, null);
            view.setTag(i);
            view.setClickable(true);
            view.setSelected(false);
            view.setOnClickListener(this);

            LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(
                    mActivity.getResources().getDimensionPixelOffset(R.dimen.quiz_option_width),
                    mActivity.getResources().getDimensionPixelOffset(R.dimen.quiz_option_height));
            params.gravity = Gravity.CENTER_HORIZONTAL;
            params.topMargin = mActivity.getResources().getDimensionPixelOffset(R.dimen.quiz_option_margin);
            mOptionLayout.addView(view, params);

            TextView text = view.findViewById(R.id.quiz_option_name);
            text.setText(options.get(i));
        }

        mQuiz = quiz;

        ImageView closeView = mContainer.findViewById(R.id.quiz_close_btn);
        closeView.setOnClickListener(this);
    }

    public void show(Quiz quiz) {
        initUI(quiz);
        startCount();
    }

    public void dismiss() {
        stopCount();
        mContainer.setVisibility(View.GONE);
    }

    public boolean isShowing() {
        return mContainer.getVisibility() == View.VISIBLE;
    }

    private void startCount() {
        mContainer.clearAnimation();
        mHandler.postDelayed(mCountRunnable, 1000);
    }

    private void stopCount() {
        mHandler.removeCallbacks(mCountRunnable);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.quiz_close_btn:
                dismiss();
                return;
        }

        if (v.getTag() instanceof Integer) {
            int id = (int) v.getTag();
            setOptionSelected(id);
            mSelectedIndex = id;
        }
    }

    private void setOptionSelected(int index) {
        int count = mOptionLayout.getChildCount();
        for (int i = 0; i < count; i++) {
            if (i == index) {
                mOptionLayout.getChildAt(i).setSelected(true);
            } else {
                mOptionLayout.getChildAt(i).setSelected(false);
            }
        }
    }
}
