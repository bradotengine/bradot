/**************************************************************************/
/*  library_bradot_audio.js                                                */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             BRADOT ENGINE                              */
/*                        https://bradotengine.org                        */
/**************************************************************************/
/* Copyright (c) 2024-present Bradot Engine contributors (see AUTHORS.md).*/
/* Copyright (c) 2014-2024 Godot Engine contributors (see AUTHORS.md).    */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

/**
 * @typedef { "disabled" | "forward" | "backward" | "pingpong" } LoopMode
 */

/**
 * @typedef {{
 *   id: string
 *   audioBuffer: AudioBuffer
 * }} SampleParams
 * @typedef {{
 *   numberOfChannels?: number
 *   sampleRate?: number
 *   loopMode?: LoopMode
 *   loopBegin?: number
 *   loopEnd?: number
 * }} SampleOptions
 */

/**
 * Represents a sample, memory-wise.
 * @class
 */
class Sample {
	/**
	 * Returns a `Sample`.
	 * @param {string} id Id of the `Sample` to get.
	 * @returns {Sample}
	 * @throws {ReferenceError} When no `Sample` is found
	 */
	static getSample(id) {
		if (!BradotAudio.samples.has(id)) {
			throw new ReferenceError(`Could not find sample "${id}"`);
		}
		return BradotAudio.samples.get(id);
	}

	/**
	 * Returns a `Sample` or `null`, if it doesn't exist.
	 * @param {string} id Id of the `Sample` to get.
	 * @returns {Sample?}
	 */
	static getSampleOrNull(id) {
		return BradotAudio.samples.get(id) ?? null;
	}

	/**
	 * Creates a `Sample` based on the params. Will register it to the
	 * `BradotAudio.samples` registry.
	 * @param {SampleParams} params Base params
	 * @param {SampleOptions} [options={{}}] Optional params
	 * @returns {Sample}
	 */
	static create(params, options = {}) {
		const sample = new BradotAudio.Sample(params, options);
		BradotAudio.samples.set(params.id, sample);
		return sample;
	}

	/**
	 * Deletes a `Sample` based on the id.
	 * @param {string} id `Sample` id to delete
	 * @returns {void}
	 */
	static delete(id) {
		BradotAudio.samples.delete(id);
	}

	/**
	 * `Sample` constructor.
	 * @param {SampleParams} params Base params
	 * @param {SampleOptions} [options={{}}] Optional params
	 */
	constructor(params, options = {}) {
		/** @type {string} */
		this.id = params.id;
		/** @type {AudioBuffer} */
		this._audioBuffer = null;
		/** @type {number} */
		this.numberOfChannels = options.numberOfChannels ?? 2;
		/** @type {number} */
		this.sampleRate = options.sampleRate ?? 44100;
		/** @type {LoopMode} */
		this.loopMode = options.loopMode ?? 'disabled';
		/** @type {number} */
		this.loopBegin = options.loopBegin ?? 0;
		/** @type {number} */
		this.loopEnd = options.loopEnd ?? 0;

		this.setAudioBuffer(params.audioBuffer);
	}

	/**
	 * Gets the audio buffer of the sample.
	 * @returns {AudioBuffer}
	 */
	getAudioBuffer() {
		return this._duplicateAudioBuffer();
	}

	/**
	 * Sets the audio buffer of the sample.
	 * @param {AudioBuffer} val The audio buffer to set.
	 * @returns {void}
	 */
	setAudioBuffer(val) {
		this._audioBuffer = val;
	}

	/**
	 * Clears the current sample.
	 * @returns {void}
	 */
	clear() {
		this.setAudioBuffer(null);
		BradotAudio.Sample.delete(this.id);
	}

	/**
	 * Returns a duplicate of the stored audio buffer.
	 * @returns {AudioBuffer}
	 */
	_duplicateAudioBuffer() {
		if (this._audioBuffer == null) {
			throw new Error('couldn\'t duplicate a null audioBuffer');
		}
		/** @type {Array<Float32Array>} */
		const channels = new Array(this._audioBuffer.numberOfChannels);
		for (let i = 0; i < this._audioBuffer.numberOfChannels; i++) {
			const channel = new Float32Array(this._audioBuffer.getChannelData(i));
			channels[i] = channel;
		}
		const buffer = BradotAudio.ctx.createBuffer(
			this.numberOfChannels,
			this._audioBuffer.length,
			this._audioBuffer.sampleRate
		);
		for (let i = 0; i < channels.length; i++) {
			buffer.copyToChannel(channels[i], i, 0);
		}
		return buffer;
	}
}

/**
 * Represents a `SampleNode` linked to a `Bus`.
 * @class
 */
class SampleNodeBus {
	/**
	 * Creates a new `SampleNodeBus`.
	 * @param {Bus} bus The bus related to the new `SampleNodeBus`.
	 * @returns {SampleNodeBus}
	 */
	static create(bus) {
		return new BradotAudio.SampleNodeBus(bus);
	}

	/**
	 * `SampleNodeBus` constructor.
	 * @param {Bus} bus The bus related to the new `SampleNodeBus`.
	 */
	constructor(bus) {
		const NUMBER_OF_WEB_CHANNELS = 6;

		/** @type {Bus} */
		this._bus = bus;

		/** @type {ChannelSplitterNode} */
		this._channelSplitter = BradotAudio.ctx.createChannelSplitter(NUMBER_OF_WEB_CHANNELS);
		/** @type {GainNode} */
		this._l = BradotAudio.ctx.createGain();
		/** @type {GainNode} */
		this._r = BradotAudio.ctx.createGain();
		/** @type {GainNode} */
		this._sl = BradotAudio.ctx.createGain();
		/** @type {GainNode} */
		this._sr = BradotAudio.ctx.createGain();
		/** @type {GainNode} */
		this._c = BradotAudio.ctx.createGain();
		/** @type {GainNode} */
		this._lfe = BradotAudio.ctx.createGain();
		/** @type {ChannelMergerNode} */
		this._channelMerger = BradotAudio.ctx.createChannelMerger(NUMBER_OF_WEB_CHANNELS);

		this._channelSplitter
			.connect(this._l, BradotAudio.WebChannel.CHANNEL_L)
			.connect(
				this._channelMerger,
				BradotAudio.WebChannel.CHANNEL_L,
				BradotAudio.WebChannel.CHANNEL_L
			);
		this._channelSplitter
			.connect(this._r, BradotAudio.WebChannel.CHANNEL_R)
			.connect(
				this._channelMerger,
				BradotAudio.WebChannel.CHANNEL_L,
				BradotAudio.WebChannel.CHANNEL_R
			);
		this._channelSplitter
			.connect(this._sl, BradotAudio.WebChannel.CHANNEL_SL)
			.connect(
				this._channelMerger,
				BradotAudio.WebChannel.CHANNEL_L,
				BradotAudio.WebChannel.CHANNEL_SL
			);
		this._channelSplitter
			.connect(this._sr, BradotAudio.WebChannel.CHANNEL_SR)
			.connect(
				this._channelMerger,
				BradotAudio.WebChannel.CHANNEL_L,
				BradotAudio.WebChannel.CHANNEL_SR
			);
		this._channelSplitter
			.connect(this._c, BradotAudio.WebChannel.CHANNEL_C)
			.connect(
				this._channelMerger,
				BradotAudio.WebChannel.CHANNEL_L,
				BradotAudio.WebChannel.CHANNEL_C
			);
		this._channelSplitter
			.connect(this._lfe, BradotAudio.WebChannel.CHANNEL_L)
			.connect(
				this._channelMerger,
				BradotAudio.WebChannel.CHANNEL_L,
				BradotAudio.WebChannel.CHANNEL_LFE
			);

		this._channelMerger.connect(this._bus.getInputNode());
	}

	/**
	 * Returns the input node.
	 * @returns {AudioNode}
	 */
	getInputNode() {
		return this._channelSplitter;
	}

	/**
	 * Returns the output node.
	 * @returns {AudioNode}
	 */
	getOutputNode() {
		return this._channelMerger;
	}

	/**
	 * Sets the volume for each (split) channel.
	 * @param {Float32Array} volume Volume array from the engine for each channel.
	 * @returns {void}
	 */
	setVolume(volume) {
		if (volume.length !== BradotAudio.MAX_VOLUME_CHANNELS) {
			throw new Error(
				`Volume length isn't "${BradotAudio.MAX_VOLUME_CHANNELS}", is ${volume.length} instead`
			);
		}
		this._l.gain.value = volume[BradotAudio.BradotChannel.CHANNEL_L] ?? 0;
		this._r.gain.value = volume[BradotAudio.BradotChannel.CHANNEL_R] ?? 0;
		this._sl.gain.value = volume[BradotAudio.BradotChannel.CHANNEL_SL] ?? 0;
		this._sr.gain.value = volume[BradotAudio.BradotChannel.CHANNEL_SR] ?? 0;
		this._c.gain.value = volume[BradotAudio.BradotChannel.CHANNEL_C] ?? 0;
		this._lfe.gain.value = volume[BradotAudio.BradotChannel.CHANNEL_LFE] ?? 0;
	}

	/**
	 * Clears the current `SampleNodeBus` instance.
	 * @returns {void}
	 */
	clear() {
		this._bus = null;
		this._channelSplitter.disconnect();
		this._channelSplitter = null;
		this._l.disconnect();
		this._l = null;
		this._r.disconnect();
		this._r = null;
		this._sl.disconnect();
		this._sl = null;
		this._sr.disconnect();
		this._sr = null;
		this._c.disconnect();
		this._c = null;
		this._lfe.disconnect();
		this._lfe = null;
		this._channelMerger.disconnect();
		this._channelMerger = null;
	}
}

/**
 * @typedef {{
 *   id: string
 *   streamObjectId: string
 *   busIndex: number
 * }} SampleNodeParams
 * @typedef {{
 *   offset?: number
 *   playbackRate?: number
 *   startTime?: number
 *   pitchScale?: number
 *   loopMode?: LoopMode
 *   volume?: Float32Array
 *   start?: boolean
 * }} SampleNodeOptions
 */

/**
 * Represents an `AudioNode` of a `Sample`.
 * @class
 */
class SampleNode {
	/**
	 * Returns a `SampleNode`.
	 * @param {string} id Id of the `SampleNode`.
	 * @returns {SampleNode}
	 * @throws {ReferenceError} When no `SampleNode` is not found
	 */
	static getSampleNode(id) {
		if (!BradotAudio.sampleNodes.has(id)) {
			throw new ReferenceError(`Could not find sample node "${id}"`);
		}
		return BradotAudio.sampleNodes.get(id);
	}

	/**
	 * Returns a `SampleNode`, returns null if not found.
	 * @param {string} id Id of the SampleNode.
	 * @returns {SampleNode?}
	 */
	static getSampleNodeOrNull(id) {
		return BradotAudio.sampleNodes.get(id) ?? null;
	}

	/**
	 * Stops a `SampleNode` by id.
	 * @param {string} id Id of the `SampleNode` to stop.
	 * @returns {void}
	 */
	static stopSampleNode(id) {
		const sampleNode = BradotAudio.SampleNode.getSampleNodeOrNull(id);
		if (sampleNode == null) {
			return;
		}
		sampleNode.stop();
	}

	/**
	 * Pauses the `SampleNode` by id.
	 * @param {string} id Id of the `SampleNode` to pause.
	 * @param {boolean} enable State of the pause
	 * @returns {void}
	 */
	static pauseSampleNode(id, enable) {
		const sampleNode = BradotAudio.SampleNode.getSampleNodeOrNull(id);
		if (sampleNode == null) {
			return;
		}
		sampleNode.pause(enable);
	}

	/**
	 * Creates a `SampleNode` based on the params. Will register the `SampleNode` to
	 * the `BradotAudio.sampleNodes` regisery.
	 * @param {SampleNodeParams} params Base params.
	 * @param {SampleNodeOptions} options Optional params.
	 * @returns {SampleNode}
	 */
	static create(params, options = {}) {
		const sampleNode = new BradotAudio.SampleNode(params, options);
		BradotAudio.sampleNodes.set(params.id, sampleNode);
		return sampleNode;
	}

	/**
	 * Deletes a `SampleNode` based on the id.
	 * @param {string} id Id of the `SampleNode` to delete.
	 * @returns {void}
	 */
	static delete(id) {
		BradotAudio.sampleNodes.delete(id);
	}

	/**
	 * @param {SampleNodeParams} params Base params
	 * @param {SampleNodeOptions} [options={{}}] Optional params
	 */
	constructor(params, options = {}) {
		/** @type {string} */
		this.id = params.id;
		/** @type {string} */
		this.streamObjectId = params.streamObjectId;
		/** @type {number} */
		this.offset = options.offset ?? 0;
		/** @type {number} */
		this._playbackPosition = options.offset;
		/** @type {number} */
		this.startTime = options.startTime ?? 0;
		/** @type {boolean} */
		this.isPaused = false;
		/** @type {boolean} */
		this.isStarted = false;
		/** @type {boolean} */
		this.isCanceled = false;
		/** @type {number} */
		this.pauseTime = 0;
		/** @type {number} */
		this._playbackRate = 44100;
		/** @type {LoopMode} */
		this.loopMode = options.loopMode ?? this.getSample().loopMode ?? 'disabled';
		/** @type {number} */
		this._pitchScale = options.pitchScale ?? 1;
		/** @type {number} */
		this._sourceStartTime = 0;
		/** @type {Map<Bus, SampleNodeBus>} */
		this._sampleNodeBuses = new Map();
		/** @type {AudioBufferSourceNode | null} */
		this._source = BradotAudio.ctx.createBufferSource();

		this._onended = null;
		/** @type {AudioWorkletNode | null} */
		this._positionWorklet = null;

		this.setPlaybackRate(options.playbackRate ?? 44100);
		this._source.buffer = this.getSample().getAudioBuffer();

		this._addEndedListener();

		const bus = BradotAudio.Bus.getBus(params.busIndex);
		const sampleNodeBus = this.getSampleNodeBus(bus);
		sampleNodeBus.setVolume(options.volume);

		this.connectPositionWorklet(options.start);
	}

	/**
	 * Gets the playback rate.
	 * @returns {number}
	 */
	getPlaybackRate() {
		return this._playbackRate;
	}

	/**
	 * Gets the playback position.
	 * @returns {number}
	 */
	getPlaybackPosition() {
		return this._playbackPosition;
	}

	/**
	 * Sets the playback rate.
	 * @param {number} val Value to set.
	 * @returns {void}
	 */
	setPlaybackRate(val) {
		this._playbackRate = val;
		this._syncPlaybackRate();
	}

	/**
	 * Gets the pitch scale.
	 * @returns {number}
	 */
	getPitchScale() {
		return this._pitchScale;
	}

	/**
	 * Sets the pitch scale.
	 * @param {number} val Value to set.
	 * @returns {void}
	 */
	setPitchScale(val) {
		this._pitchScale = val;
		this._syncPlaybackRate();
	}

	/**
	 * Returns the linked `Sample`.
	 * @returns {Sample}
	 */
	getSample() {
		return BradotAudio.Sample.getSample(this.streamObjectId);
	}

	/**
	 * Returns the output node.
	 * @returns {AudioNode}
	 */
	getOutputNode() {
		return this._source;
	}

	/**
	 * Starts the `SampleNode`.
	 * @returns {void}
	 */
	start() {
		if (this.isStarted) {
			return;
		}
		this._resetSourceStartTime();
		this._source.start(this.startTime, this.offset);
		this.isStarted = true;
	}

	/**
	 * Stops the `SampleNode`.
	 * @returns {void}
	 */
	stop() {
		this.clear();
	}

	/**
	 * Restarts the `SampleNode`.
	 */
	restart() {
		this.isPaused = false;
		this.pauseTime = 0;
		this._resetSourceStartTime();
		this._restart();
	}

	/**
	 * Pauses the `SampleNode`.
	 * @param {boolean} [enable=true] State of the pause.
	 * @returns {void}
	 */
	pause(enable = true) {
		if (enable) {
			this._pause();
			return;
		}

		this._unpause();
	}

	/**
	 * Connects an AudioNode to the output node of this `SampleNode`.
	 * @param {AudioNode} node AudioNode to connect.
	 * @returns {void}
	 */
	connect(node) {
		return this.getOutputNode().connect(node);
	}

	/**
	 * Sets the volumes of the `SampleNode` for each buses passed in parameters.
	 * @param {Array<Bus>} buses
	 * @param {Float32Array} volumes
	 */
	setVolumes(buses, volumes) {
		for (let busIdx = 0; busIdx < buses.length; busIdx++) {
			const sampleNodeBus = this.getSampleNodeBus(buses[busIdx]);
			sampleNodeBus.setVolume(
				volumes.slice(
					busIdx * BradotAudio.MAX_VOLUME_CHANNELS,
					(busIdx * BradotAudio.MAX_VOLUME_CHANNELS) + BradotAudio.MAX_VOLUME_CHANNELS
				)
			);
		}
	}

	/**
	 * Returns the SampleNodeBus based on the bus in parameters.
	 * @param {Bus} bus Bus to get the SampleNodeBus from.
	 * @returns {SampleNodeBus}
	 */
	getSampleNodeBus(bus) {
		if (!this._sampleNodeBuses.has(bus)) {
			const sampleNodeBus = BradotAudio.SampleNodeBus.create(bus);
			this._sampleNodeBuses.set(bus, sampleNodeBus);
			this._source.connect(sampleNodeBus.getInputNode());
		}
		return this._sampleNodeBuses.get(bus);
	}

	/**
	 * Sets up and connects the source to the BradotPositionReportingProcessor
	 * If the worklet module is not loaded in, it will be added
	 */
	connectPositionWorklet(start) {
		try {
			this._positionWorklet = this.createPositionWorklet();
			this._source.connect(this._positionWorklet);
			if (start) {
				this.start();
			}
		} catch (error) {
			if (error?.name !== 'InvalidStateError') {
				throw error;
			}
			const path = BradotConfig.locate_file('bradot.audio.position.worklet.js');
			BradotAudio.ctx.audioWorklet
				.addModule(path)
				.then(() => {
					if (!this.isCanceled) {
						this._positionWorklet = this.createPositionWorklet();
						this._source.connect(this._positionWorklet);
						if (start) {
							this.start();
						}
					}
				}).catch((addModuleError) => {
					BradotRuntime.error('Failed to create PositionWorklet.', addModuleError);
				});
		}
	}

	/**
	 * Creates the AudioWorkletProcessor used to track playback position.
	 * @returns {AudioWorkletNode}
	 */
	createPositionWorklet() {
		const worklet = new AudioWorkletNode(
			BradotAudio.ctx,
			'bradot-position-reporting-processor'
		);
		worklet.port.onmessage = (event) => {
			switch (event.data['type']) {
			case 'position':
				this._playbackPosition = (parseInt(event.data.data, 10) / this.getSample().sampleRate) + this.offset;
				break;
			default:
				// Do nothing.
			}
		};
		return worklet;
	}

	/**
	 * Clears the `SampleNode`.
	 * @returns {void}
	 */
	clear() {
		this.isCanceled = true;
		this.isPaused = false;
		this.pauseTime = 0;

		if (this._source != null) {
			this._source.removeEventListener('ended', this._onended);
			this._onended = null;
			if (this.isStarted) {
				this._source.stop();
			}
			this._source.disconnect();
			this._source = null;
		}

		for (const sampleNodeBus of this._sampleNodeBuses.values()) {
			sampleNodeBus.clear();
		}
		this._sampleNodeBuses.clear();

		if (this._positionWorklet) {
			this._positionWorklet.disconnect();
			this._positionWorklet.port.onmessage = null;
			this._positionWorklet = null;
		}

		BradotAudio.SampleNode.delete(this.id);
	}

	/**
	 * Resets the source start time
	 * @returns {void}
	 */
	_resetSourceStartTime() {
		this._sourceStartTime = BradotAudio.ctx.currentTime;
	}

	/**
	 * Syncs the `AudioNode` playback rate based on the `SampleNode` playback rate and pitch scale.
	 * @returns {void}
	 */
	_syncPlaybackRate() {
		this._source.playbackRate.value = this.getPlaybackRate() * this.getPitchScale();
	}

	/**
	 * Restarts the `SampleNode`.
	 * Honors `isPaused` and `pauseTime`.
	 * @returns {void}
	 */
	_restart() {
		if (this._source != null) {
			this._source.disconnect();
		}
		this._source = BradotAudio.ctx.createBufferSource();
		this._source.buffer = this.getSample().getAudioBuffer();

		// Make sure that we connect the new source to the sample node bus.
		for (const sampleNodeBus of this._sampleNodeBuses.values()) {
			this.connect(sampleNodeBus.getInputNode());
		}

		this._addEndedListener();
		const pauseTime = this.isPaused
			? this.pauseTime
			: 0;
		this.connectPositionWorklet();
		this._source.start(this.startTime, this.offset + pauseTime);
		this.isStarted = true;
	}

	/**
	 * Pauses the `SampleNode`.
	 * @returns {void}
	 */
	_pause() {
		this.isPaused = true;
		this.pauseTime = (BradotAudio.ctx.currentTime - this._sourceStartTime) / this.getPlaybackRate();
		this._source.stop();
	}

	/**
	 * Unpauses the `SampleNode`.
	 * @returns {void}
	 */
	_unpause() {
		this._restart();
		this.isPaused = false;
		this.pauseTime = 0;
	}

	/**
	 * Adds an "ended" listener to the source node to repeat it if necessary.
	 * @returns {void}
	 */
	_addEndedListener() {
		if (this._onended != null) {
			this._source.removeEventListener('ended', this._onended);
		}

		/** @type {SampleNode} */
		// eslint-disable-next-line consistent-this
		const self = this;
		this._onended = (_) => {
			if (self.isPaused) {
				return;
			}

			switch (self.getSample().loopMode) {
			case 'disabled': {
				const id = this.id;
				self.stop();
				if (BradotAudio.sampleFinishedCallback != null) {
					const idCharPtr = BradotRuntime.allocString(id);
					BradotAudio.sampleFinishedCallback(idCharPtr);
					BradotRuntime.free(idCharPtr);
				}
			} break;
			case 'forward':
			case 'backward':
				self.restart();
				break;
			default:
				// do nothing
			}
		};
		this._source.addEventListener('ended', this._onended);
	}
}

/**
 * Collection of nodes to represents a Bradot Engine audio bus.
 * @class
 */
class Bus {
	/**
	 * Returns the number of registered buses.
	 * @returns {number}
	 */
	static getCount() {
		return BradotAudio.buses.length;
	}

	/**
	 * Sets the number of registered buses.
	 * Will delete buses if lower than the current number.
	 * @param {number} val Count of registered buses.
	 * @returns {void}
	 */
	static setCount(val) {
		const buses = BradotAudio.buses;
		if (val === buses.length) {
			return;
		}

		if (val < buses.length) {
			// TODO: what to do with nodes connected to the deleted buses?
			const deletedBuses = buses.slice(val);
			for (let i = 0; i < deletedBuses.length; i++) {
				const deletedBus = deletedBuses[i];
				deletedBus.clear();
			}
			BradotAudio.buses = buses.slice(0, val);
			return;
		}

		for (let i = BradotAudio.buses.length; i < val; i++) {
			BradotAudio.Bus.create();
		}
	}

	/**
	 * Returns a `Bus` based on it's index number.
	 * @param {number} index
	 * @returns {Bus}
	 * @throws {ReferenceError} If the index value is outside the registry.
	 */
	static getBus(index) {
		if (index < 0 || index >= BradotAudio.buses.length) {
			throw new ReferenceError(`invalid bus index "${index}"`);
		}
		return BradotAudio.buses[index];
	}

	/**
	 * Returns a `Bus` based on it's index number. Returns null if it doesn't exist.
	 * @param {number} index
	 * @returns {Bus?}
	 */
	static getBusOrNull(index) {
		if (index < 0 || index >= BradotAudio.buses.length) {
			return null;
		}
		return BradotAudio.buses[index];
	}

	/**
	 * Move a bus from an index to another.
	 * @param {number} fromIndex From index
	 * @param {number} toIndex To index
	 * @returns {void}
	 */
	static move(fromIndex, toIndex) {
		const movedBus = BradotAudio.Bus.getBusOrNull(fromIndex);
		if (movedBus == null) {
			return;
		}
		const buses = BradotAudio.buses.filter((_, i) => i !== fromIndex);
		// Inserts at index.
		buses.splice(toIndex - 1, 0, movedBus);
		BradotAudio.buses = buses;
	}

	/**
	 * Adds a new bus at the specified index.
	 * @param {number} index Index to add a new bus.
	 * @returns {void}
	 */
	static addAt(index) {
		const newBus = BradotAudio.Bus.create();
		if (index !== newBus.getId()) {
			BradotAudio.Bus.move(newBus.getId(), index);
		}
	}

	/**
	 * Creates a `Bus` and registers it.
	 * @returns {Bus}
	 */
	static create() {
		const newBus = new BradotAudio.Bus();
		const isFirstBus = BradotAudio.buses.length === 0;
		BradotAudio.buses.push(newBus);
		if (isFirstBus) {
			newBus.setSend(null);
		} else {
			newBus.setSend(BradotAudio.Bus.getBus(0));
		}
		return newBus;
	}

	/**
	 * `Bus` constructor.
	 */
	constructor() {
		/** @type {Set<SampleNode>} */
		this._sampleNodes = new Set();
		/** @type {boolean} */
		this.isSolo = false;
		/** @type {Bus?} */
		this._send = null;

		/** @type {GainNode} */
		this._gainNode = BradotAudio.ctx.createGain();
		/** @type {GainNode} */
		this._soloNode = BradotAudio.ctx.createGain();
		/** @type {GainNode} */
		this._muteNode = BradotAudio.ctx.createGain();

		this._gainNode
			.connect(this._soloNode)
			.connect(this._muteNode);
	}

	/**
	 * Returns the current id of the bus (its index).
	 * @returns {number}
	 */
	getId() {
		return BradotAudio.buses.indexOf(this);
	}

	/**
	 * Returns the bus volume db value.
	 * @returns {number}
	 */
	getVolumeDb() {
		return BradotAudio.linear_to_db(this._gainNode.gain.value);
	}

	/**
	 * Sets the bus volume db value.
	 * @param {number} val Value to set
	 * @returns {void}
	 */
	setVolumeDb(val) {
		const linear = BradotAudio.db_to_linear(val);
		if (isFinite(linear)) {
			this._gainNode.gain.value = linear;
		}
	}

	/**
	 * Returns the "send" bus.
	 * If null, this bus sends its contents directly to the output.
	 * If not null, this bus sends its contents to another bus.
	 * @returns {Bus?}
	 */
	getSend() {
		return this._send;
	}

	/**
	 * Sets the "send" bus.
	 * If null, this bus sends its contents directly to the output.
	 * If not null, this bus sends its contents to another bus.
	 *
	 * **Note:** if null, `getId()` must be equal to 0. Otherwise, it will throw.
	 * @param {Bus?} val
	 * @returns {void}
	 * @throws {Error} When val is `null` and `getId()` isn't equal to 0
	 */
	setSend(val) {
		this._send = val;
		if (val == null) {
			if (this.getId() == 0) {
				this.getOutputNode().connect(BradotAudio.ctx.destination);
				return;
			}
			throw new Error(
				`Cannot send to "${val}" without the bus being at index 0 (current index: ${this.getId()})`
			);
		}
		this.connect(val);
	}

	/**
	 * Returns the input node of the bus.
	 * @returns {AudioNode}
	 */
	getInputNode() {
		return this._gainNode;
	}

	/**
	 * Returns the output node of the bus.
	 * @returns {AudioNode}
	 */
	getOutputNode() {
		return this._muteNode;
	}

	/**
	 * Sets the mute status of the bus.
	 * @param {boolean} enable
	 */
	mute(enable) {
		this._muteNode.gain.value = enable ? 0 : 1;
	}

	/**
	 * Sets the solo status of the bus.
	 * @param {boolean} enable
	 */
	solo(enable) {
		if (this.isSolo === enable) {
			return;
		}

		if (enable) {
			if (BradotAudio.busSolo != null && BradotAudio.busSolo !== this) {
				BradotAudio.busSolo._disableSolo();
			}
			this._enableSolo();
			return;
		}

		this._disableSolo();
	}

	/**
	 * Wrapper to simply add a sample node to the bus.
	 * @param {SampleNode} sampleNode `SampleNode` to remove
	 * @returns {void}
	 */
	addSampleNode(sampleNode) {
		this._sampleNodes.add(sampleNode);
		sampleNode.getOutputNode().connect(this.getInputNode());
	}

	/**
	 * Wrapper to simply remove a sample node from the bus.
	 * @param {SampleNode} sampleNode `SampleNode` to remove
	 * @returns {void}
	 */
	removeSampleNode(sampleNode) {
		this._sampleNodes.delete(sampleNode);
		sampleNode.getOutputNode().disconnect();
	}

	/**
	 * Wrapper to simply connect to another bus.
	 * @param {Bus} bus
	 * @returns {void}
	 */
	connect(bus) {
		if (bus == null) {
			throw new Error('cannot connect to null bus');
		}
		this.getOutputNode().disconnect();
		this.getOutputNode().connect(bus.getInputNode());
		return bus;
	}

	/**
	 * Clears the current bus.
	 * @returns {void}
	 */
	clear() {
		BradotAudio.buses = BradotAudio.buses.filter((v) => v !== this);
	}

	_syncSampleNodes() {
		const sampleNodes = Array.from(this._sampleNodes);
		for (let i = 0; i < sampleNodes.length; i++) {
			const sampleNode = sampleNodes[i];
			sampleNode.getOutputNode().disconnect();
			sampleNode.getOutputNode().connect(this.getInputNode());
		}
	}

	/**
	 * Process to enable solo.
	 * @returns {void}
	 */
	_enableSolo() {
		this.isSolo = true;
		BradotAudio.busSolo = this;
		this._soloNode.gain.value = 1;
		const otherBuses = BradotAudio.buses.filter(
			(otherBus) => otherBus !== this
		);
		for (let i = 0; i < otherBuses.length; i++) {
			const otherBus = otherBuses[i];
			otherBus._soloNode.gain.value = 0;
		}
	}

	/**
	 * Process to disable solo.
	 * @returns {void}
	 */
	_disableSolo() {
		this.isSolo = false;
		BradotAudio.busSolo = null;
		this._soloNode.gain.value = 1;
		const otherBuses = BradotAudio.buses.filter(
			(otherBus) => otherBus !== this
		);
		for (let i = 0; i < otherBuses.length; i++) {
			const otherBus = otherBuses[i];
			otherBus._soloNode.gain.value = 1;
		}
	}
}

const _BradotAudio = {
	$BradotAudio__deps: ['$BradotRuntime', '$BradotOS'],
	$BradotAudio: {
		/**
		 * Max number of volume channels.
		 */
		MAX_VOLUME_CHANNELS: 8,

		/**
		 * Represents the index of each sound channel relative to the engine.
		 */
		BradotChannel: Object.freeze({
			CHANNEL_L: 0,
			CHANNEL_R: 1,
			CHANNEL_C: 3,
			CHANNEL_LFE: 4,
			CHANNEL_RL: 5,
			CHANNEL_RR: 6,
			CHANNEL_SL: 7,
			CHANNEL_SR: 8,
		}),

		/**
		 * Represents the index of each sound channel relative to the Web Audio API.
		 */
		WebChannel: Object.freeze({
			CHANNEL_L: 0,
			CHANNEL_R: 1,
			CHANNEL_SL: 2,
			CHANNEL_SR: 3,
			CHANNEL_C: 4,
			CHANNEL_LFE: 5,
		}),

		// `Sample` class
		/**
		 * Registry of `Sample`s.
		 * @type {Map<string, Sample>}
		 */
		samples: null,
		Sample,

		// `SampleNodeBus` class
		SampleNodeBus,

		// `SampleNode` class
		/**
		 * Registry of `SampleNode`s.
		 * @type {Map<string, SampleNode>}
		 */
		sampleNodes: null,
		SampleNode,

		// `Bus` class
		/**
		 * Registry of `Bus`es.
		 * @type {Array<Bus>}
		 */
		buses: null,
		/**
		 * Reference to the current bus in solo mode.
		 * @type {Bus | null}
		 */
		busSolo: null,
		Bus,

		/**
		 * Callback to signal that a sample has finished.
		 * @type {(playbackObjectIdPtr: number) => void | null}
		 */
		sampleFinishedCallback: null,

		/** @type {AudioContext} */
		ctx: null,
		input: null,
		driver: null,
		interval: 0,

		/**
		 * Converts linear volume to Db.
		 * @param {number} linear Linear value to convert.
		 * @returns {number}
		 */
		linear_to_db: function (linear) {
			// eslint-disable-next-line no-loss-of-precision
			return Math.log(linear) * 8.6858896380650365530225783783321;
		},
		/**
		 * Converts Db volume to linear.
		 * @param {number} db Db value to convert.
		 * @returns {number}
		 */
		db_to_linear: function (db) {
			// eslint-disable-next-line no-loss-of-precision
			return Math.exp(db * 0.11512925464970228420089957273422);
		},

		init: function (mix_rate, latency, onstatechange, onlatencyupdate) {
			// Initialize classes static values.
			BradotAudio.samples = new Map();
			BradotAudio.sampleNodes = new Map();
			BradotAudio.buses = [];
			BradotAudio.busSolo = null;

			const opts = {};
			// If mix_rate is 0, let the browser choose.
			if (mix_rate) {
				BradotAudio.sampleRate = mix_rate;
				opts['sampleRate'] = mix_rate;
			}
			// Do not specify, leave 'interactive' for good performance.
			// opts['latencyHint'] = latency / 1000;
			const ctx = new (window.AudioContext || window.webkitAudioContext)(opts);
			BradotAudio.ctx = ctx;
			ctx.onstatechange = function () {
				let state = 0;
				switch (ctx.state) {
				case 'suspended':
					state = 0;
					break;
				case 'running':
					state = 1;
					break;
				case 'closed':
					state = 2;
					break;
				default:
					// Do nothing.
				}
				onstatechange(state);
			};
			ctx.onstatechange(); // Immediately notify state.
			// Update computed latency
			BradotAudio.interval = setInterval(function () {
				let computed_latency = 0;
				if (ctx.baseLatency) {
					computed_latency += BradotAudio.ctx.baseLatency;
				}
				if (ctx.outputLatency) {
					computed_latency += BradotAudio.ctx.outputLatency;
				}
				onlatencyupdate(computed_latency);
			}, 1000);
			BradotOS.atexit(BradotAudio.close_async);
			return ctx.destination.channelCount;
		},

		create_input: function (callback) {
			if (BradotAudio.input) {
				return 0; // Already started.
			}
			function gotMediaInput(stream) {
				try {
					BradotAudio.input = BradotAudio.ctx.createMediaStreamSource(stream);
					callback(BradotAudio.input);
				} catch (e) {
					BradotRuntime.error('Failed creating input.', e);
				}
			}
			if (navigator.mediaDevices && navigator.mediaDevices.getUserMedia) {
				navigator.mediaDevices.getUserMedia({
					'audio': true,
				}).then(gotMediaInput, function (e) {
					BradotRuntime.error('Error getting user media.', e);
				});
			} else {
				if (!navigator.getUserMedia) {
					navigator.getUserMedia = navigator.webkitGetUserMedia || navigator.mozGetUserMedia;
				}
				if (!navigator.getUserMedia) {
					BradotRuntime.error('getUserMedia not available.');
					return 1;
				}
				navigator.getUserMedia({
					'audio': true,
				}, gotMediaInput, function (e) {
					BradotRuntime.print(e);
				});
			}
			return 0;
		},

		close_async: function (resolve, reject) {
			const ctx = BradotAudio.ctx;
			BradotAudio.ctx = null;
			// Audio was not initialized.
			if (!ctx) {
				resolve();
				return;
			}
			// Remove latency callback
			if (BradotAudio.interval) {
				clearInterval(BradotAudio.interval);
				BradotAudio.interval = 0;
			}
			// Disconnect input, if it was started.
			if (BradotAudio.input) {
				BradotAudio.input.disconnect();
				BradotAudio.input = null;
			}
			// Disconnect output
			let closed = Promise.resolve();
			if (BradotAudio.driver) {
				closed = BradotAudio.driver.close();
			}
			closed.then(function () {
				return ctx.close();
			}).then(function () {
				ctx.onstatechange = null;
				resolve();
			}).catch(function (e) {
				ctx.onstatechange = null;
				BradotRuntime.error('Error closing AudioContext', e);
				resolve();
			});
		},

		/**
		 * Triggered when a sample node needs to start.
		 * @param {string} playbackObjectId The unique id of the sample playback
		 * @param {string} streamObjectId The unique id of the stream
		 * @param {number} busIndex Index of the bus currently binded to the sample playback
		 * @param {SampleNodeOptions} startOptions Optional params
		 * @returns {void}
		 */
		start_sample: function (
			playbackObjectId,
			streamObjectId,
			busIndex,
			startOptions
		) {
			BradotAudio.SampleNode.stopSampleNode(playbackObjectId);
			BradotAudio.SampleNode.create(
				{
					busIndex,
					id: playbackObjectId,
					streamObjectId,
				},
				startOptions
			);
		},

		/**
		 * Triggered when a sample node needs to be stopped.
		 * @param {string} playbackObjectId Id of the sample playback
		 * @returns {void}
		 */
		stop_sample: function (playbackObjectId) {
			BradotAudio.SampleNode.stopSampleNode(playbackObjectId);
		},

		/**
		 * Triggered when a sample node needs to be paused or unpaused.
		 * @param {string} playbackObjectId Id of the sample playback
		 * @param {boolean} pause State of the pause
		 * @returns {void}
		 */
		sample_set_pause: function (playbackObjectId, pause) {
			BradotAudio.SampleNode.pauseSampleNode(playbackObjectId, pause);
		},

		/**
		 * Triggered when a sample node needs its pitch scale to be updated.
		 * @param {string} playbackObjectId Id of the sample playback
		 * @param {number} pitchScale Pitch scale of the sample playback
		 * @returns {void}
		 */
		update_sample_pitch_scale: function (playbackObjectId, pitchScale) {
			const sampleNode = BradotAudio.SampleNode.getSampleNodeOrNull(playbackObjectId);
			if (sampleNode == null) {
				return;
			}
			sampleNode.setPitchScale(pitchScale);
		},

		/**
		 * Triggered when a sample node volumes need to be updated.
		 * @param {string} playbackObjectId Id of the sample playback
		 * @param {Array<number>} busIndexes Indexes of the buses that need to be updated
		 * @param {Float32Array} volumes Array of the volumes
		 * @returns {void}
		 */
		sample_set_volumes_linear: function (playbackObjectId, busIndexes, volumes) {
			const sampleNode = BradotAudio.SampleNode.getSampleNodeOrNull(playbackObjectId);
			if (sampleNode == null) {
				return;
			}
			const buses = busIndexes.map((busIndex) => BradotAudio.Bus.getBus(busIndex));
			sampleNode.setVolumes(buses, volumes);
		},

		/**
		 * Triggered when the bus count changes.
		 * @param {number} count Number of buses
		 * @returns {void}
		 */
		set_sample_bus_count: function (count) {
			BradotAudio.Bus.setCount(count);
		},

		/**
		 * Triggered when a bus needs to be removed.
		 * @param {number} index Bus index
		 * @returns {void}
		 */
		remove_sample_bus: function (index) {
			const bus = BradotAudio.Bus.getBusOrNull(index);
			if (bus == null) {
				return;
			}
			bus.clear();
		},

		/**
		 * Triggered when a bus needs to be at the desired position.
		 * @param {number} atPos Position to add the bus
		 * @returns {void}
		 */
		add_sample_bus: function (atPos) {
			BradotAudio.Bus.addAt(atPos);
		},

		/**
		 * Triggered when a bus needs to be moved.
		 * @param {number} busIndex Index of the bus to move
		 * @param {number} toPos Index of the new position of the bus
		 * @returns {void}
		 */
		move_sample_bus: function (busIndex, toPos) {
			BradotAudio.Bus.move(busIndex, toPos);
		},

		/**
		 * Triggered when the "send" value of a bus changes.
		 * @param {number} busIndex Index of the bus to update the "send" value
		 * @param {number} sendIndex Index of the bus that is the new "send"
		 * @returns {void}
		 */
		set_sample_bus_send: function (busIndex, sendIndex) {
			const bus = BradotAudio.Bus.getBusOrNull(busIndex);
			if (bus == null) {
				// Cannot send from an invalid bus.
				return;
			}
			let targetBus = BradotAudio.Bus.getBusOrNull(sendIndex);
			if (targetBus == null) {
				// Send to master.
				targetBus = BradotAudio.Bus.getBus(0);
			}
			bus.setSend(targetBus);
		},

		/**
		 * Triggered when a bus needs its volume db to be updated.
		 * @param {number} busIndex Index of the bus to update its volume db
		 * @param {number} volumeDb Volume of the bus
		 * @returns {void}
		 */
		set_sample_bus_volume_db: function (busIndex, volumeDb) {
			const bus = BradotAudio.Bus.getBusOrNull(busIndex);
			if (bus == null) {
				return;
			}
			bus.setVolumeDb(volumeDb);
		},

		/**
		 * Triggered when a bus needs to update its solo status
		 * @param {number} busIndex Index of the bus to update its solo status
		 * @param {boolean} enable Status of the solo
		 * @returns {void}
		 */
		set_sample_bus_solo: function (busIndex, enable) {
			const bus = BradotAudio.Bus.getBusOrNull(busIndex);
			if (bus == null) {
				return;
			}
			bus.solo(enable);
		},

		/**
		 * Triggered when a bus needs to update its mute status
		 * @param {number} busIndex Index of the bus to update its mute status
		 * @param {boolean} enable Status of the mute
		 * @returns {void}
		 */
		set_sample_bus_mute: function (busIndex, enable) {
			const bus = BradotAudio.Bus.getBusOrNull(busIndex);
			if (bus == null) {
				return;
			}
			bus.mute(enable);
		},
	},

	bradot_audio_is_available__sig: 'i',
	bradot_audio_is_available__proxy: 'sync',
	bradot_audio_is_available: function () {
		if (!(window.AudioContext || window.webkitAudioContext)) {
			return 0;
		}
		return 1;
	},

	bradot_audio_has_worklet__proxy: 'sync',
	bradot_audio_has_worklet__sig: 'i',
	bradot_audio_has_worklet: function () {
		return BradotAudio.ctx && BradotAudio.ctx.audioWorklet ? 1 : 0;
	},

	bradot_audio_has_script_processor__proxy: 'sync',
	bradot_audio_has_script_processor__sig: 'i',
	bradot_audio_has_script_processor: function () {
		return BradotAudio.ctx && BradotAudio.ctx.createScriptProcessor ? 1 : 0;
	},

	bradot_audio_init__proxy: 'sync',
	bradot_audio_init__sig: 'iiiii',
	bradot_audio_init: function (
		p_mix_rate,
		p_latency,
		p_state_change,
		p_latency_update
	) {
		const statechange = BradotRuntime.get_func(p_state_change);
		const latencyupdate = BradotRuntime.get_func(p_latency_update);
		const mix_rate = BradotRuntime.getHeapValue(p_mix_rate, 'i32');
		const channels = BradotAudio.init(
			mix_rate,
			p_latency,
			statechange,
			latencyupdate
		);
		BradotRuntime.setHeapValue(p_mix_rate, BradotAudio.ctx.sampleRate, 'i32');
		return channels;
	},

	bradot_audio_resume__proxy: 'sync',
	bradot_audio_resume__sig: 'v',
	bradot_audio_resume: function () {
		if (BradotAudio.ctx && BradotAudio.ctx.state !== 'running') {
			BradotAudio.ctx.resume();
		}
	},

	bradot_audio_input_start__proxy: 'sync',
	bradot_audio_input_start__sig: 'i',
	bradot_audio_input_start: function () {
		return BradotAudio.create_input(function (input) {
			input.connect(BradotAudio.driver.get_node());
		});
	},

	bradot_audio_input_stop__proxy: 'sync',
	bradot_audio_input_stop__sig: 'v',
	bradot_audio_input_stop: function () {
		if (BradotAudio.input) {
			const tracks = BradotAudio.input['mediaStream']['getTracks']();
			for (let i = 0; i < tracks.length; i++) {
				tracks[i]['stop']();
			}
			BradotAudio.input.disconnect();
			BradotAudio.input = null;
		}
	},

	bradot_audio_sample_stream_is_registered__proxy: 'sync',
	bradot_audio_sample_stream_is_registered__sig: 'ii',
	/**
	 * Returns if the sample stream is registered
	 * @param {number} streamObjectIdStrPtr Pointer of the streamObjectId
	 * @returns {number}
	 */
	bradot_audio_sample_stream_is_registered: function (streamObjectIdStrPtr) {
		const streamObjectId = BradotRuntime.parseString(streamObjectIdStrPtr);
		return Number(BradotAudio.Sample.getSampleOrNull(streamObjectId) != null);
	},

	bradot_audio_sample_register_stream__proxy: 'sync',
	bradot_audio_sample_register_stream__sig: 'viiiiiii',
	/**
	 * Registers a stream.
	 * @param {number} streamObjectIdStrPtr StreamObjectId pointer
	 * @param {number} framesPtr Frames pointer
	 * @param {number} framesTotal Frames total value
	 * @param {number} loopModeStrPtr Loop mode pointer
	 * @param {number} loopBegin Loop begin value
	 * @param {number} loopEnd Loop end value
	 * @returns {void}
	 */
	bradot_audio_sample_register_stream: function (
		streamObjectIdStrPtr,
		framesPtr,
		framesTotal,
		loopModeStrPtr,
		loopBegin,
		loopEnd
	) {
		const BYTES_PER_FLOAT32 = 4;
		const streamObjectId = BradotRuntime.parseString(streamObjectIdStrPtr);
		const loopMode = BradotRuntime.parseString(loopModeStrPtr);
		const numberOfChannels = 2;
		const sampleRate = BradotAudio.ctx.sampleRate;

		/** @type {Float32Array} */
		const subLeft = BradotRuntime.heapSub(HEAPF32, framesPtr, framesTotal);
		/** @type {Float32Array} */
		const subRight = BradotRuntime.heapSub(
			HEAPF32,
			framesPtr + framesTotal * BYTES_PER_FLOAT32,
			framesTotal
		);

		const audioBuffer = BradotAudio.ctx.createBuffer(
			numberOfChannels,
			framesTotal,
			sampleRate
		);
		audioBuffer.copyToChannel(new Float32Array(subLeft), 0, 0);
		audioBuffer.copyToChannel(new Float32Array(subRight), 1, 0);

		BradotAudio.Sample.create(
			{
				id: streamObjectId,
				audioBuffer,
			},
			{
				loopBegin,
				loopEnd,
				loopMode,
				numberOfChannels,
				sampleRate,
			}
		);
	},

	bradot_audio_sample_unregister_stream__proxy: 'sync',
	bradot_audio_sample_unregister_stream__sig: 'vi',
	/**
	 * Unregisters a stream.
	 * @param {number} streamObjectIdStrPtr StreamObjectId pointer
	 * @returns {void}
	 */
	bradot_audio_sample_unregister_stream: function (streamObjectIdStrPtr) {
		const streamObjectId = BradotRuntime.parseString(streamObjectIdStrPtr);
		const sample = BradotAudio.Sample.getSampleOrNull(streamObjectId);
		if (sample != null) {
			sample.clear();
		}
	},

	bradot_audio_sample_start__proxy: 'sync',
	bradot_audio_sample_start__sig: 'viiiifi',
	/**
	 * Starts a sample.
	 * @param {number} playbackObjectIdStrPtr Playback object id pointer
	 * @param {number} streamObjectIdStrPtr Stream object id pointer
	 * @param {number} busIndex Bus index
	 * @param {number} offset Sample offset
	 * @param {number} pitchScale Pitch scale
	 * @param {number} volumePtr Volume pointer
	 * @returns {void}
	 */
	bradot_audio_sample_start: function (
		playbackObjectIdStrPtr,
		streamObjectIdStrPtr,
		busIndex,
		offset,
		pitchScale,
		volumePtr
	) {
		/** @type {string} */
		const playbackObjectId = BradotRuntime.parseString(playbackObjectIdStrPtr);
		/** @type {string} */
		const streamObjectId = BradotRuntime.parseString(streamObjectIdStrPtr);
		/** @type {Float32Array} */
		const volume = BradotRuntime.heapSub(HEAPF32, volumePtr, 8);
		/** @type {SampleNodeOptions} */
		const startOptions = {
			offset,
			volume,
			playbackRate: 1,
			pitchScale,
			start: true,
		};
		BradotAudio.start_sample(
			playbackObjectId,
			streamObjectId,
			busIndex,
			startOptions
		);
	},

	bradot_audio_sample_stop__proxy: 'sync',
	bradot_audio_sample_stop__sig: 'vi',
	/**
	 * Stops a sample from playing.
	 * @param {number} playbackObjectIdStrPtr Playback object id pointer
	 * @returns {void}
	 */
	bradot_audio_sample_stop: function (playbackObjectIdStrPtr) {
		const playbackObjectId = BradotRuntime.parseString(playbackObjectIdStrPtr);
		BradotAudio.stop_sample(playbackObjectId);
	},

	bradot_audio_sample_set_pause__proxy: 'sync',
	bradot_audio_sample_set_pause__sig: 'vii',
	/**
	 * Sets the pause state of a sample.
	 * @param {number} playbackObjectIdStrPtr Playback object id pointer
	 * @param {number} pause Pause state
	 */
	bradot_audio_sample_set_pause: function (playbackObjectIdStrPtr, pause) {
		const playbackObjectId = BradotRuntime.parseString(playbackObjectIdStrPtr);
		BradotAudio.sample_set_pause(playbackObjectId, Boolean(pause));
	},

	bradot_audio_sample_is_active__proxy: 'sync',
	bradot_audio_sample_is_active__sig: 'ii',
	/**
	 * Returns if the sample is active.
	 * @param {number} playbackObjectIdStrPtr Playback object id pointer
	 * @returns {number}
	 */
	bradot_audio_sample_is_active: function (playbackObjectIdStrPtr) {
		const playbackObjectId = BradotRuntime.parseString(playbackObjectIdStrPtr);
		return Number(BradotAudio.sampleNodes.has(playbackObjectId));
	},

	bradot_audio_get_sample_playback_position__proxy: 'sync',
	bradot_audio_get_sample_playback_position__sig: 'di',
	/**
	 * Returns the position of the playback position.
	 * @param {number} playbackObjectIdStrPtr Playback object id pointer
	 * @returns {number}
	 */
	bradot_audio_get_sample_playback_position: function (playbackObjectIdStrPtr) {
		const playbackObjectId = BradotRuntime.parseString(playbackObjectIdStrPtr);
		const sampleNode = BradotAudio.SampleNode.getSampleNodeOrNull(playbackObjectId);
		if (sampleNode == null) {
			return 0;
		}
		return sampleNode.getPlaybackPosition();
	},

	bradot_audio_sample_update_pitch_scale__proxy: 'sync',
	bradot_audio_sample_update_pitch_scale__sig: 'vii',
	/**
	 * Updates the pitch scale of a sample.
	 * @param {number} playbackObjectIdStrPtr Playback object id pointer
	 * @param {number} pitchScale Pitch scale value
	 * @returns {void}
	 */
	bradot_audio_sample_update_pitch_scale: function (
		playbackObjectIdStrPtr,
		pitchScale
	) {
		const playbackObjectId = BradotRuntime.parseString(playbackObjectIdStrPtr);
		BradotAudio.update_sample_pitch_scale(playbackObjectId, pitchScale);
	},

	bradot_audio_sample_set_volumes_linear__proxy: 'sync',
	bradot_audio_sample_set_volumes_linear__sig: 'vii',
	/**
	 * Sets the volumes linear of each mentioned bus for the sample.
	 * @param {number} playbackObjectIdStrPtr Playback object id pointer
	 * @param {number} busesPtr Buses array pointer
	 * @param {number} busesSize Buses array size
	 * @param {number} volumesPtr Volumes array pointer
	 * @param {number} volumesSize Volumes array size
	 * @returns {void}
	 */
	bradot_audio_sample_set_volumes_linear: function (
		playbackObjectIdStrPtr,
		busesPtr,
		busesSize,
		volumesPtr,
		volumesSize
	) {
		/** @type {string} */
		const playbackObjectId = BradotRuntime.parseString(playbackObjectIdStrPtr);

		/** @type {Uint32Array} */
		const buses = BradotRuntime.heapSub(HEAP32, busesPtr, busesSize);
		/** @type {Float32Array} */
		const volumes = BradotRuntime.heapSub(HEAPF32, volumesPtr, volumesSize);

		BradotAudio.sample_set_volumes_linear(
			playbackObjectId,
			Array.from(buses),
			volumes
		);
	},

	bradot_audio_sample_bus_set_count__proxy: 'sync',
	bradot_audio_sample_bus_set_count__sig: 'vi',
	/**
	 * Sets the bus count.
	 * @param {number} count Bus count
	 * @returns {void}
	 */
	bradot_audio_sample_bus_set_count: function (count) {
		BradotAudio.set_sample_bus_count(count);
	},

	bradot_audio_sample_bus_remove__proxy: 'sync',
	bradot_audio_sample_bus_remove__sig: 'vi',
	/**
	 * Removes a bus.
	 * @param {number} index Index of the bus to remove
	 * @returns {void}
	 */
	bradot_audio_sample_bus_remove: function (index) {
		BradotAudio.remove_sample_bus(index);
	},

	bradot_audio_sample_bus_add__proxy: 'sync',
	bradot_audio_sample_bus_add__sig: 'vi',
	/**
	 * Adds a bus at the defined position.
	 * @param {number} atPos Position to add the bus
	 * @returns {void}
	 */
	bradot_audio_sample_bus_add: function (atPos) {
		BradotAudio.add_sample_bus(atPos);
	},

	bradot_audio_sample_bus_move__proxy: 'sync',
	bradot_audio_sample_bus_move__sig: 'vii',
	/**
	 * Moves the bus from a position to another.
	 * @param {number} fromPos Position of the bus to move
	 * @param {number} toPos Final position of the bus
	 * @returns {void}
	 */
	bradot_audio_sample_bus_move: function (fromPos, toPos) {
		BradotAudio.move_sample_bus(fromPos, toPos);
	},

	bradot_audio_sample_bus_set_send__proxy: 'sync',
	bradot_audio_sample_bus_set_send__sig: 'vii',
	/**
	 * Sets the "send" of a bus.
	 * @param {number} bus Position of the bus to set the send
	 * @param {number} sendIndex Position of the "send" bus
	 * @returns {void}
	 */
	bradot_audio_sample_bus_set_send: function (bus, sendIndex) {
		BradotAudio.set_sample_bus_send(bus, sendIndex);
	},

	bradot_audio_sample_bus_set_volume_db__proxy: 'sync',
	bradot_audio_sample_bus_set_volume_db__sig: 'vii',
	/**
	 * Sets the volume db of a bus.
	 * @param {number} bus Position of the bus to set the volume db
	 * @param {number} volumeDb Volume db to set
	 * @returns {void}
	 */
	bradot_audio_sample_bus_set_volume_db: function (bus, volumeDb) {
		BradotAudio.set_sample_bus_volume_db(bus, volumeDb);
	},

	bradot_audio_sample_bus_set_solo__proxy: 'sync',
	bradot_audio_sample_bus_set_solo__sig: 'vii',
	/**
	 * Sets the state of solo for a bus
	 * @param {number} bus Position of the bus to set the solo state
	 * @param {number} enable State of the solo
	 * @returns {void}
	 */
	bradot_audio_sample_bus_set_solo: function (bus, enable) {
		BradotAudio.set_sample_bus_solo(bus, Boolean(enable));
	},

	bradot_audio_sample_bus_set_mute__proxy: 'sync',
	bradot_audio_sample_bus_set_mute__sig: 'vii',
	/**
	 * Sets the state of mute for a bus
	 * @param {number} bus Position of the bus to set the mute state
	 * @param {number} enable State of the mute
	 * @returns {void}
	 */
	bradot_audio_sample_bus_set_mute: function (bus, enable) {
		BradotAudio.set_sample_bus_mute(bus, Boolean(enable));
	},

	bradot_audio_sample_set_finished_callback__proxy: 'sync',
	bradot_audio_sample_set_finished_callback__sig: 'vi',
	/**
	 * Sets the finished callback
	 * @param {Number} callbackPtr Finished callback pointer
	 * @returns {void}
	 */
	bradot_audio_sample_set_finished_callback: function (callbackPtr) {
		BradotAudio.sampleFinishedCallback = BradotRuntime.get_func(callbackPtr);
	},
};

autoAddDeps(_BradotAudio, '$BradotAudio');
mergeInto(LibraryManager.library, _BradotAudio);

/**
 * The AudioWorklet API driver, used when threads are available.
 */
const BradotAudioWorklet = {
	$BradotAudioWorklet__deps: ['$BradotAudio', '$BradotConfig'],
	$BradotAudioWorklet: {
		promise: null,
		worklet: null,
		ring_buffer: null,

		create: function (channels) {
			const path = BradotConfig.locate_file('bradot.audio.worklet.js');
			BradotAudioWorklet.promise = BradotAudio.ctx.audioWorklet
				.addModule(path)
				.then(function () {
					BradotAudioWorklet.worklet = new AudioWorkletNode(
						BradotAudio.ctx,
						'bradot-processor',
						{
							outputChannelCount: [channels],
						}
					);
					return Promise.resolve();
				});
			BradotAudio.driver = BradotAudioWorklet;
		},

		start: function (in_buf, out_buf, state) {
			BradotAudioWorklet.promise.then(function () {
				const node = BradotAudioWorklet.worklet;
				node.connect(BradotAudio.ctx.destination);
				node.port.postMessage({
					'cmd': 'start',
					'data': [state, in_buf, out_buf],
				});
				node.port.onmessage = function (event) {
					BradotRuntime.error(event.data);
				};
			});
		},

		start_no_threads: function (
			p_out_buf,
			p_out_size,
			out_callback,
			p_in_buf,
			p_in_size,
			in_callback
		) {
			function RingBuffer() {
				let wpos = 0;
				let rpos = 0;
				let pending_samples = 0;
				const wbuf = new Float32Array(p_out_size);

				function send(port) {
					if (pending_samples === 0) {
						return;
					}
					const buffer = BradotRuntime.heapSub(HEAPF32, p_out_buf, p_out_size);
					const size = buffer.length;
					const tot_sent = pending_samples;
					out_callback(wpos, pending_samples);
					if (wpos + pending_samples >= size) {
						const high = size - wpos;
						wbuf.set(buffer.subarray(wpos, size));
						pending_samples -= high;
						wpos = 0;
					}
					if (pending_samples > 0) {
						wbuf.set(
							buffer.subarray(wpos, wpos + pending_samples),
							tot_sent - pending_samples
						);
					}
					port.postMessage({ 'cmd': 'chunk', 'data': wbuf.subarray(0, tot_sent) });
					wpos += pending_samples;
					pending_samples = 0;
				}
				this.receive = function (recv_buf) {
					const buffer = BradotRuntime.heapSub(HEAPF32, p_in_buf, p_in_size);
					const from = rpos;
					let to_write = recv_buf.length;
					let high = 0;
					if (rpos + to_write >= p_in_size) {
						high = p_in_size - rpos;
						buffer.set(recv_buf.subarray(0, high), rpos);
						to_write -= high;
						rpos = 0;
					}
					if (to_write) {
						buffer.set(recv_buf.subarray(high, to_write), rpos);
					}
					in_callback(from, recv_buf.length);
					rpos += to_write;
				};
				this.consumed = function (size, port) {
					pending_samples += size;
					send(port);
				};
			}
			BradotAudioWorklet.ring_buffer = new RingBuffer();
			BradotAudioWorklet.promise.then(function () {
				const node = BradotAudioWorklet.worklet;
				const buffer = BradotRuntime.heapSlice(HEAPF32, p_out_buf, p_out_size);
				node.connect(BradotAudio.ctx.destination);
				node.port.postMessage({
					'cmd': 'start_nothreads',
					'data': [buffer, p_in_size],
				});
				node.port.onmessage = function (event) {
					if (!BradotAudioWorklet.worklet) {
						return;
					}
					if (event.data['cmd'] === 'read') {
						const read = event.data['data'];
						BradotAudioWorklet.ring_buffer.consumed(
							read,
							BradotAudioWorklet.worklet.port
						);
					} else if (event.data['cmd'] === 'input') {
						const buf = event.data['data'];
						if (buf.length > p_in_size) {
							BradotRuntime.error('Input chunk is too big');
							return;
						}
						BradotAudioWorklet.ring_buffer.receive(buf);
					} else {
						BradotRuntime.error(event.data);
					}
				};
			});
		},

		get_node: function () {
			return BradotAudioWorklet.worklet;
		},

		close: function () {
			return new Promise(function (resolve, reject) {
				if (BradotAudioWorklet.promise === null) {
					return;
				}
				const p = BradotAudioWorklet.promise;
				p.then(function () {
					BradotAudioWorklet.worklet.port.postMessage({
						'cmd': 'stop',
						'data': null,
					});
					BradotAudioWorklet.worklet.disconnect();
					BradotAudioWorklet.worklet.port.onmessage = null;
					BradotAudioWorklet.worklet = null;
					BradotAudioWorklet.promise = null;
					resolve();
				}).catch(function (err) {
					// Aborted?
					BradotRuntime.error(err);
				});
			});
		},
	},

	bradot_audio_worklet_create__proxy: 'sync',
	bradot_audio_worklet_create__sig: 'ii',
	bradot_audio_worklet_create: function (channels) {
		try {
			BradotAudioWorklet.create(channels);
		} catch (e) {
			BradotRuntime.error('Error starting AudioDriverWorklet', e);
			return 1;
		}
		return 0;
	},

	bradot_audio_worklet_start__proxy: 'sync',
	bradot_audio_worklet_start__sig: 'viiiii',
	bradot_audio_worklet_start: function (
		p_in_buf,
		p_in_size,
		p_out_buf,
		p_out_size,
		p_state
	) {
		const out_buffer = BradotRuntime.heapSub(HEAPF32, p_out_buf, p_out_size);
		const in_buffer = BradotRuntime.heapSub(HEAPF32, p_in_buf, p_in_size);
		const state = BradotRuntime.heapSub(HEAP32, p_state, 4);
		BradotAudioWorklet.start(in_buffer, out_buffer, state);
	},

	bradot_audio_worklet_start_no_threads__proxy: 'sync',
	bradot_audio_worklet_start_no_threads__sig: 'viiiiii',
	bradot_audio_worklet_start_no_threads: function (
		p_out_buf,
		p_out_size,
		p_out_callback,
		p_in_buf,
		p_in_size,
		p_in_callback
	) {
		const out_callback = BradotRuntime.get_func(p_out_callback);
		const in_callback = BradotRuntime.get_func(p_in_callback);
		BradotAudioWorklet.start_no_threads(
			p_out_buf,
			p_out_size,
			out_callback,
			p_in_buf,
			p_in_size,
			in_callback
		);
	},

	bradot_audio_worklet_state_wait__sig: 'iiii',
	bradot_audio_worklet_state_wait: function (
		p_state,
		p_idx,
		p_expected,
		p_timeout
	) {
		Atomics.wait(HEAP32, (p_state >> 2) + p_idx, p_expected, p_timeout);
		return Atomics.load(HEAP32, (p_state >> 2) + p_idx);
	},

	bradot_audio_worklet_state_add__sig: 'iiii',
	bradot_audio_worklet_state_add: function (p_state, p_idx, p_value) {
		return Atomics.add(HEAP32, (p_state >> 2) + p_idx, p_value);
	},

	bradot_audio_worklet_state_get__sig: 'iii',
	bradot_audio_worklet_state_get: function (p_state, p_idx) {
		return Atomics.load(HEAP32, (p_state >> 2) + p_idx);
	},
};

autoAddDeps(BradotAudioWorklet, '$BradotAudioWorklet');
mergeInto(LibraryManager.library, BradotAudioWorklet);

/*
 * The ScriptProcessorNode API, used when threads are disabled.
 */
const BradotAudioScript = {
	$BradotAudioScript__deps: ['$BradotAudio'],
	$BradotAudioScript: {
		script: null,

		create: function (buffer_length, channel_count) {
			BradotAudioScript.script = BradotAudio.ctx.createScriptProcessor(
				buffer_length,
				2,
				channel_count
			);
			BradotAudio.driver = BradotAudioScript;
			return BradotAudioScript.script.bufferSize;
		},

		start: function (p_in_buf, p_in_size, p_out_buf, p_out_size, onprocess) {
			BradotAudioScript.script.onaudioprocess = function (event) {
				// Read input
				const inb = BradotRuntime.heapSub(HEAPF32, p_in_buf, p_in_size);
				const input = event.inputBuffer;
				if (BradotAudio.input) {
					const inlen = input.getChannelData(0).length;
					for (let ch = 0; ch < 2; ch++) {
						const data = input.getChannelData(ch);
						for (let s = 0; s < inlen; s++) {
							inb[s * 2 + ch] = data[s];
						}
					}
				}

				// Let Bradot process the input/output.
				onprocess();

				// Write the output.
				const outb = BradotRuntime.heapSub(HEAPF32, p_out_buf, p_out_size);
				const output = event.outputBuffer;
				const channels = output.numberOfChannels;
				for (let ch = 0; ch < channels; ch++) {
					const data = output.getChannelData(ch);
					// Loop through samples and assign computed values.
					for (let sample = 0; sample < data.length; sample++) {
						data[sample] = outb[sample * channels + ch];
					}
				}
			};
			BradotAudioScript.script.connect(BradotAudio.ctx.destination);
		},

		get_node: function () {
			return BradotAudioScript.script;
		},

		close: function () {
			return new Promise(function (resolve, reject) {
				BradotAudioScript.script.disconnect();
				BradotAudioScript.script.onaudioprocess = null;
				BradotAudioScript.script = null;
				resolve();
			});
		},
	},

	bradot_audio_script_create__proxy: 'sync',
	bradot_audio_script_create__sig: 'iii',
	bradot_audio_script_create: function (buffer_length, channel_count) {
		const buf_len = BradotRuntime.getHeapValue(buffer_length, 'i32');
		try {
			const out_len = BradotAudioScript.create(buf_len, channel_count);
			BradotRuntime.setHeapValue(buffer_length, out_len, 'i32');
		} catch (e) {
			BradotRuntime.error('Error starting AudioDriverScriptProcessor', e);
			return 1;
		}
		return 0;
	},

	bradot_audio_script_start__proxy: 'sync',
	bradot_audio_script_start__sig: 'viiiii',
	bradot_audio_script_start: function (
		p_in_buf,
		p_in_size,
		p_out_buf,
		p_out_size,
		p_cb
	) {
		const onprocess = BradotRuntime.get_func(p_cb);
		BradotAudioScript.start(
			p_in_buf,
			p_in_size,
			p_out_buf,
			p_out_size,
			onprocess
		);
	},
};

autoAddDeps(BradotAudioScript, '$BradotAudioScript');
mergeInto(LibraryManager.library, BradotAudioScript);