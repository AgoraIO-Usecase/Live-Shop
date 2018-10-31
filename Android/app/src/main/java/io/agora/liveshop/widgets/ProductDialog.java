package io.agora.liveshop.widgets;

import android.app.Activity;
import android.os.Handler;
import android.view.SurfaceView;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.TextView;

import java.util.Iterator;
import java.util.List;
import java.util.Map;

import io.agora.liveshop.R;
import io.agora.liveshop.data.Product;
import io.agora.rtc.Constants;
import io.agora.rtc.RtcEngine;
import io.agora.rtc.video.VideoCanvas;

public class ProductDialog implements View.OnClickListener {
    private static final int DOUBLE_CLICK_THRESHOLD = 400;

    private Activity mActivity;
    private View mContainer;
    private FrameLayout mSmallVideoLayout;
    private FrameLayout mLargeVideoLayout;
    private TextView mTitleText;
    private TextView mVariantText;
    private TextView mDetailText;
    private TextView mOriginalPriceText;
    private TextView mGroupDealText;

    private int mProductRemoteId;
    private RtcEngine mRtcEngine;

    private boolean mFirstClicked = false;
    private Handler mHandler;
    private boolean mShouldShowLargeVideo = false;

    private Runnable mDoubleClickInvalidator = new Runnable() {
        @Override
        public void run() {
            mFirstClicked = false;
        }
    };

    public ProductDialog(Activity activity, View view, RtcEngine rtcEngine) {
        mActivity = activity;
        mContainer = view;
        mRtcEngine = rtcEngine;
        mHandler = new Handler(mActivity.getMainLooper());
        initUI();
    }

    private void initUI() {
        mLargeVideoLayout = mActivity.findViewById(R.id.remote_product_video_big);
        mLargeVideoLayout.setOnClickListener(this);
        mSmallVideoLayout = mContainer.findViewById(R.id.product_video_small);
        mTitleText = mContainer.findViewById(R.id.product_title);
        mVariantText = mContainer.findViewById(R.id.product_variant);
        mDetailText = mContainer.findViewById(R.id.product_detail);
        mOriginalPriceText = mContainer.findViewById(R.id.original_price_value);
        mGroupDealText = mContainer.findViewById(R.id.group_deal_value);
    }

    private void init(Product product) {
        if (product == null) {
            return;
        }

        mTitleText.setText(product.getTitle());

        StringBuilder builder = new StringBuilder();
        Map<String, List<String>> variants = product.getVariants();
        Iterator<Map.Entry<String, List<String>>> iterator = variants.entrySet().iterator();
        while (iterator.hasNext()) {
            Map.Entry<String, List<String>> entry = iterator.next();
            builder.append(entry.getKey()).append(": ");
            for (String value : entry.getValue()) {
                builder.append(value).append(", ");
            }
            builder.append('\n');
        }
        int varLength = builder.length();
        builder.deleteCharAt(varLength - 1);
        mVariantText.setText(builder.toString());

        builder.delete(0, builder.length() - 1);
        builder.append("Product Details - ").append(product.getSold()).append(" sold");
        mDetailText.setText(builder.toString());

        mOriginalPriceText.setText(product.getCurrency() + product.getOriginalPrice());
        mGroupDealText.setText(product.getCurrency() + product.getGroupPrice());

        mProductRemoteId = product.getStreamId();
        mSmallVideoLayout.removeAllViews();
        mSmallVideoLayout.setOnClickListener(this);
        SurfaceView surfaceView = setupRemoteView(false);
        mRtcEngine.setRemoteVideoStreamType(mProductRemoteId, Constants.VIDEO_STREAM_LOW);
        mSmallVideoLayout.addView(surfaceView);
    }

    public void show(Product product) {
        init(product);
        mContainer.setVisibility(View.VISIBLE);
    }

    public void dismiss() {
        mContainer.setVisibility(View.GONE);
        if (mRtcEngine != null) {
            mRtcEngine.setupRemoteVideo(null);
        }
    }

    public boolean isShowing() {
        return mContainer.getVisibility() == View.VISIBLE;
    }

    @Override
    public void onClick(View v) {
        int id = v.getId();
        if (id == R.id.product_video_small || id == R.id.remote_product_video_big) {
            if (!mFirstClicked) {
                mFirstClicked = true;
                mHandler.postDelayed(mDoubleClickInvalidator, DOUBLE_CLICK_THRESHOLD);
            } else {
                switchRemoteView();
                mHandler.removeCallbacks(mDoubleClickInvalidator);
                mFirstClicked = false;
            }
        }
    }

    private SurfaceView setupRemoteView(boolean clear) {
        SurfaceView surfaceView = null;
        if (!clear) {
            surfaceView = RtcEngine.CreateRendererView(mActivity);
            surfaceView.setZOrderOnTop(true);
        }

        mRtcEngine.setupRemoteVideo(new VideoCanvas(surfaceView, VideoCanvas.RENDER_MODE_HIDDEN, mProductRemoteId));
        return surfaceView;
    }

    private void switchRemoteView() {
        setupRemoteView(true);
        SurfaceView surfaceView = setupRemoteView(false);

        if (mShouldShowLargeVideo) {
            if (mSmallVideoLayout.getChildCount() > 0) {
                mSmallVideoLayout.removeAllViews();
            }
            mRtcEngine.setRemoteVideoStreamType(mProductRemoteId, Constants.VIDEO_STREAM_HIGH);
            mLargeVideoLayout.addView(surfaceView);
        } else {
            if (mLargeVideoLayout.getChildCount() > 0) {
                mLargeVideoLayout.removeAllViews();
            }
            mRtcEngine.setRemoteVideoStreamType(mProductRemoteId, Constants.VIDEO_STREAM_LOW);
            mSmallVideoLayout.addView(surfaceView);
        }

        mShouldShowLargeVideo = !mShouldShowLargeVideo;
    }
}
